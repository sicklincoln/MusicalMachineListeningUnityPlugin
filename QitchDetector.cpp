//(c) Nick Collins 2019 all rights reserved
//this file 8/1/05 by Nicholas M. Collins after Brown/Puckette
//updated for fftw 13/12/06
//converted to SC fft interface, 26 March 2011


#include "QitchDetector.h"
#include "QitchData2048.h"

#define TWOPI 6.28318530717952646f


//float g_fmin;
//int g_qbands;
//float * g_qfreqs;
//float * g_kernels;

//non windowed FFT for the signal x[n] because the kernels are pre windowed
//float * g_hanning;

//for pitch search, MATLAB calculated
//sieve= round(24*log(1:11)/log(2))
//amps= fliplr(0.6:0.04:1.0);

int g_sieve[11] = {0,24,38,48,56,62,67,72,76,80,83};
float g_amps[11] ={1,0.96,0.92,0.88,0.84,0.8,0.76,0.72,0.68,0.64,0.6};

//116 is near 5000 Hz in this , 90 is 2500Hz or so




QitchDetector::QitchDetector(float sr,int windowsize=4096) {
    
    int i;
	
    ///////constant Q data in buffer passed in
    float * pdata;
    
    //more generally, could recalculate kernels to cover many SR and window size options, say 4 common sample rates * 4 window options. Could auto generate .h file based on those kernels
    //will run twice as fast at 88.2kHz
    if( (fabs(sr-44100)<0.1) || (fabs(sr-88200)<0.1) ) {
        
       if(windowsize==2048)
       pdata = qdata2048sr44100;
        else
        pdata = qdata4096sr44100;
        
    } else  {

        if(windowsize==2048)
            pdata = qdata2048sr48000;
        else
            pdata = qdata4096sr48000;
    }
    
	//get Q data
	
	int SR = (int)pdata[0];
	int fftN = (int)pdata[1];
	
    //assert(sr==SR);
    //assert(fftN==2048);
    
    
    stft = new MMLLSTFT(windowsize,512,0); //Rectangular (NO) windowing
    
	int numbands= (int)pdata[2];
	
	m_qbands=numbands;
    
	//int cumulsize= (int)pdata[3];
	
	//printf("SR %d N %d bands %d cumulsize %d \n",m_SR, m_N, m_qbands, cumulsize);
	
	//if(g_SR != SR){
	//
	//		g_SR=SR;
	//		g_Nyquist=(int)(SR/2);
	//		g_framespersec= (float)g_overlap/g_SR;
	//		g_freqperbin= (float)g_SR/(float)g_N;
	//
	//	};
	
	//other globals like g_N assumed correct for now
	//all FFTs are taken as a multiple of 1024
	m_SR = sr;
	m_N = fftN;
	m_fftscale = 1.0/(2.0*fftN); //altivec 1.0/(2.0*fftN);
	m_freqperbin = (float)SR/(float)fftN;
	
	//constants for efficiency
	m_twopioverN= TWOPI/(float)fftN;
	realb=cos(m_twopioverN);
	imagb=sin(m_twopioverN);
	

	float * qfreqs = new float[numbands]; // (float*)RTAlloc(world, numbands * sizeof(float));
	int * startindex = new int[numbands]; //(int*)RTAlloc(world, numbands * sizeof(int));
	int * numindices = new int[numbands]; //(int*)RTAlloc(world, numbands * sizeof(int));
	float ** speckernelvals = new float *[numbands]; // (float**)RTAlloc(world, numbands * sizeof(float*));
	float * qmags= new float[numbands]; //(float*)RTAlloc(world, numbands * sizeof(float));
	
	//load data
	int bufpos=3; //4;
	
	//printf("%d %p %d %p \n",i,&(pdata[bufpos]),bufpos,pdata);
	
	//can be made more efficient with pointers
	for (i=0;i<numbands; ++i) {
		
		//printf("%d %p %d %p \n",i,&(pdata[bufpos]),bufpos,pdata);
		
		//freq startind cumul numvals vals'
		qfreqs[i]= pdata[bufpos];
		startindex[i]= (int) pdata[bufpos+1];
		numindices[i]= (int) pdata[bufpos+2]; //+3
		
		//int specind= pdata[bufpos+2]; //cumulative position into this buffer
		//m_cumulindices[i]=specind;
		
		//printf("%d startind %d numind %d cumul %d \n",i, m_startindex[i], m_numindices[i], m_cumulindices[i]);
		bufpos+=3; //4;
		
		speckernelvals[i]= pdata+bufpos;
		
		//printf("%d %p %d %p %p \n",i,&(pdata[bufpos]),bufpos,pdata+bufpos, speckernelvals[i]);
		
		/*
		 for (j=0;j<m_numindices[i]; ++j) {
         
         m_speckernelvals[specind+j]= pdata[bufpos+j];
         //if(pdata[bufpos+j]>1000) printf("%d big %f indtarget %d indsource %d",i,pdata[bufpos+j],specind+j,bufpos+j);
		 }*/
		
		bufpos+= numindices[i];
	}
	
	m_qfreqs=qfreqs;
	m_startindex= startindex;
	m_numindices= numindices;
	m_speckernelvals= speckernelvals;
	m_qmags= qmags;
	
	
	/////storing complex numbers from previous frames for instananeous frequency calculation
	m_topqcandidate=numbands-(g_sieve[10])-1; //85
	float tempfreq= m_qfreqs[m_topqcandidate];
	m_ifbins=((int)ceil((tempfreq/(m_freqperbin))+0.5))+1; //cover yourself for safety
	
	//printf("numbinsstored %d tempfreq %f topcand %d \n",m_ifbins,tempfreq, m_topqcandidate); //more info!
	
	//if input amp template can correct search comb
	
	for (i=0;i<11;++i)
		m_amps[i]= g_amps[i];
	
//	uint32 ampbufnum = (uint32)ZIN0(4);
//	if (!((ampbufnum > world->mNumSndBufs) || ampbufnum<0)) {
//		SndBuf *buf2 = world->mSndBufs + ampbufnum;
//		
//		bufsize = buf2->samples;
//		
//		pdata= buf2->data;
//		
//		if(bufsize==11) {
//			for (i=0;i<11;++i)
//				m_amps[i]= pdata[i];
//		}
//	}
	
    //kr (in: 0, databufnum, ampThreshold: 0.01, algoflag: 1, ampbufnum, minfreq: 0, maxfreq: 2500)
    
	m_minfreq = 30; //0; //ZIN0(5);
	m_maxfreq = 2000; //2500; //ZIN0(6);
	
	//search qfreqs
	m_minqband= 0;
	m_maxqband=m_topqcandidate;
	
	for(i=0;i<numbands; ++i) {
		
		if(qfreqs[i]>=m_minfreq) {m_minqband=i; break;}
		
	}
	
	for(i=numbands-1;i>=0; --i) {
		
		if(qfreqs[i]<=m_maxfreq) {m_maxqband=i; break;}
		
	}
	
	//unecessary, already true m_maxqband= sc_min(m_topqcandidate, m_maxqband);
	//m_minqband= sc_min(m_minqband, m_maxqband); //necessary test if input stupid
	
    if(m_maxqband<m_minqband) m_minqband = m_maxqband;
    
	//printf("minfreq %f maxfreq %f minqband %d maxqband %d \n", m_minfreq, m_maxfreq, m_minqband,m_maxqband);
	
    m_midipitch = 69;
	m_currfreq=440;
	m_hasfreq=0;

}


QitchDetector::~QitchDetector() {
    
    delete stft;
	
	delete [] m_qfreqs;
	delete []  m_startindex;
	delete []  m_numindices;
	//delete []  m_cumulindices);
	delete []  m_speckernelvals;
	
	//delete []  m_store[0];
	//delete []  m_store[1];
    
}





void QitchDetector::Calculate(float * input,int n) {
	
	int i,j;
	
	int fftN = m_N;
	
    int ready = stft->next(input,n);
    
    if(ready) {
    
	float * fftbuf = stft->complex; //m_FFTBuf;
	float * powers = stft->powers; 
    
    //check threshold using powers
	float ampthresh = 1; //0.01; //ZIN0(2); //*2048?
	
	bool ampok=false;
	
	for (j = 0; j < fftN; ++j) {
		if (powers[j] >= ampthresh) {
			ampok = true;
			break;
		}
	}
	
	if(ampok) {
		
		
	
        
		//will probably want to store phase first
		
		// Squared Absolute so get power
		//for (i=0; i<g_N; i+=2)
		//		//i>>1 is i/2
		//		fftbuf[i>>1] = (fftbuf[i] * fftbuf[i]) + (fftbuf[i+1] * fftbuf[i+1]);
		//
		
		//amortise state changes:
		
		///////////////////////////////////////////////////////////////
		//constant Q conversion, only need magnitudes
		int qtodo= m_qbands;
		
		float * qfreqs= m_qfreqs;
		int * startindex= m_startindex;
		int * numindices= m_numindices;
		float ** speckernelvals= m_speckernelvals ;
		
		float * qmags = m_qmags;
		
		//int cumul=0;
		
		float magtotal=0.0;
		
		//printf("here 2 %p %p %p \n",speckernelvals, speckernelvals[0], speckernelvals[0]-6);
		
		for (i=0; i<qtodo; ++i) {
			
			float realsum=0.0;
			float imagsum=0.0;
			
			int start= startindex[i];
			int end=start+numindices[i];
			
			float * readbase= speckernelvals[i]-start; //+(m_cumulindices[i])-start;
			
			//printf("%d %p %p %p \n",i, speckernelvals[i], speckernelvals[i]-start, readbase);
			
			for (j=start; j<end; ++j) {
				float mult= readbase[j];
				
				//Altivec version
				//realsum+= mult*fftbuf[2*j];
				//imagsum+= mult*fftbuf[2*j+1];
				
				//fftw version
				//realsum+= mult*fftbuf[j];
				//imagsum+= mult*fftbuf[fftN-j];
				
				//sc fft version
				realsum+= mult*fftbuf[2*j];
				imagsum+= mult*fftbuf[2*j+1];
			}
			
			//scale here by 1/(2*g_N)
			
			//sclaing unecessary
			//realsum*=m_fftscale;
			//imagsum*=m_fftscale;
			
			qmags[i]= realsum*realsum+imagsum*imagsum;
			magtotal+=qmags[i];
			//if(i>70) printf("%d %f   ",i,qmags[i]);
			
		}
		//printf("\n");
		
		/////////////////////////////////////////////////////////
		float max=0.0;
		int maxindex=0;
		
		//done as per Pitch UGen now
		//float intensitycheck = ZIN0(2); //intensity check
		
		//only bother to test if amplitude is sufficient
		//printf("intensity %f check %f \n",magtotal, intensitycheck);
		
		//if(magtotal<intensitycheck) {m_hasfreq=0;}
		//else {
		
		float * pamps= m_amps;
		
		m_hasfreq=1; //could turn off if too close to call...won't bother for now
		
		//pitch detection by cross correlation, only check roots up to 2000 or so, also don't need guard element then!
		
		//can check even less if use minqband, qmaxband
		
		//int minqband= ZIN0(5);
		//int maxqband= sc_min(m_topqcandidate, ZIN0(6));
		
		//for (i=0; i<m_topqcandidate; ++i) {
		
		for (i=m_minqband; i<m_maxqband; ++i) {
			
			float sum=0.0;
			for (j=0; j<11; ++j) {
				sum+= pamps[j]*qmags[i+g_sieve[j]];
			}
			
			if(sum>max) {max=sum; maxindex=i;
                //printf("maxsum %f maxind %d \n",max, maxindex);
			}
			
		}
		
		//printf("pitch %f \n",qfreqs[maxindex]);
		
		float pitchcheck = 1; //ZIN0(3);
		
		if(pitchcheck<0.5) { m_currfreq= qfreqs[maxindex];}
		else {
			
			//////////////////////////////////////////////////////////INSTANTANEOUS FREQUENCY TRACK
			
			int k= (int)((qfreqs[maxindex]/m_freqperbin)+0.5);
			
			//printf("check k %f %f %f %d \n",qfreqs[maxindex],m_freqperbin,(qfreqs[maxindex]/m_freqperbin)+0.5,k);
			
			
			//k can't be zero else trouble
			
			//Xhk=0.5*(F.data(k,ii)-0.5*F.data(k+1,ii)-0.5*F.data(k-1,ii));
			//    Xhk2= 0.5*exp(j*2*pi*k/F.N)*(F.data(k,ii)- (0.5*exp(j*2*pi/F.N)*F.data(k+1,ii)) - (0.5*exp(-j*2*pi/F.N)*F.data(k-1,ii)));
			//
			//    theta2= angle(Xhk2); %atan(imag(Xhk2)/real(Xhk2));
			//    theta= angle(Xhk); %atan(imag(Xhk)/real(Xhk));
			//
			//    w(ii)= 44100*(abs(theta2-theta))/(2*pi);
			//
			//
			
			//instantaneous frequency correction
			float Xhkreal, Xhkimag, Xhk2real, Xhk2imag;
			
			//Altivec
			//Xhkreal=0.5*((fftbuf[2*k])-(0.5*fftbuf[2*(k+1)])-(0.5*fftbuf[2*(k-1)]));
			//Xhkimag=0.5*((fftbuf[2*k+1])-(0.5*fftbuf[2*(k+1)+1])-(0.5*fftbuf[2*(k-1)+1]));
			
			
			Xhkreal=0.5*((fftbuf[k])-(0.5*fftbuf[k+1])-(0.5*fftbuf[k-1]));
			Xhkimag=0.5*((fftbuf[fftN- k])-(0.5*fftbuf[fftN- (k+1)])-(0.5*fftbuf[fftN- (k-1)]));
			
			
			//complex exponentials to calculate a= exp(j*2*pi*k/F.N)   b= exp(j*2*pi/F.N)  c= exp(-j*2*pi/F.N)
			//float areal= cos(TWOPI*k/g_N);
			//float aimag= sin(TWOPI*k/g_N);
			
			//		float breal= cos(TWOPI/g_N);
			//			float bimag= sin(TWOPI/g_N);
			//
			//			float creal= breal;
			//			float cimag= -bimag;
			//
			//			float tmpreal= fftbuf[2*k] - (0.5*((breal*fftbuf[2*(k+1)]) - (bimag*fftbuf[2*(k+1)+1]))) - (0.5*((creal*fftbuf[2*(k-1)]) - (cimag*fftbuf[2*(k-1)+1])));
			//			float tmpimag= fftbuf[2*k+1] - (0.5*((breal*fftbuf[2*(k+1)+1]) + (bimag*fftbuf[2*(k+1)]))) - (0.5*((creal*fftbuf[2*(k-1)+1]) + (cimag*fftbuf[2*(k-1)])));
			//
			float calc= (m_twopioverN)*k;
			float areal= cos(calc);
			float aimag= sin(calc);
			
			float breal= realb;
			float bimag= imagb;
			
			
			//float tmpreal= fftbuf[2*k] - (0.5*((breal*fftbuf[2*(k+1)]) - (bimag*fftbuf[2*(k+1)+1]))) - (0.5*((breal*fftbuf[2*(k-1)]) + (bimag*fftbuf[2*(k-1)+1])));
			//float tmpimag= fftbuf[2*k+1] - (0.5*((breal*fftbuf[2*(k+1)+1]) + (bimag*fftbuf[2*(k+1)]))) - (0.5*((breal*fftbuf[2*(k-1)+1]) - (bimag*fftbuf[2*(k-1)])));
			
			
			float tmpreal= fftbuf[k] - (0.5*((breal*fftbuf[k+1]) - (bimag*fftbuf[fftN- (k+1)]))) - (0.5*((breal*fftbuf[k-1]) + (bimag*fftbuf[fftN- (k-1)])));
			float tmpimag= fftbuf[fftN-k] - (0.5*((breal*fftbuf[fftN- (k+1)]) + (bimag*fftbuf[k+1]))) - (0.5*((breal*fftbuf[fftN- (k-1)]) - (bimag*fftbuf[k-1])));
			
			
			Xhk2real= 0.5*(areal*tmpreal- aimag*tmpimag);
			Xhk2imag= 0.5*(areal*tmpimag+ aimag*tmpreal);
			
			//float Xhk2= 0.5*exp(j*2*pi*k/F.N)*(F.data(k,ii)- (0.5*exp(j*2*pi/F.N)*F.data(k+1,ii)) - (0.5*exp(-j*2*pi/F.N)*F.data(k-1,ii)));
			
			float theta2= atan(Xhk2imag/Xhk2real);
			float theta= atan(Xhkimag/Xhkreal);
			
			float freq= ((float)m_SR)*(fabs(theta2-theta))/(TWOPI);
			
			//printf("do you believe freq? %d max %f min %f result %f\n",k,m_maxfreq, m_minfreq, freq);
			
			//check no dodgy answers
			if((freq<m_minfreq) || (freq>m_maxfreq)) {m_hasfreq=0;}
			else
				m_currfreq = freq;
			
		}
		
		
        //printf("testfreq %f\n",m_currfreq);
        
        m_midipitch = (log2(m_currfreq/440.f) * 12) + 69; //(((log2(m_currfreq/440.0)) * 12) + 69); //as MIDI notes
        
    }	else {m_hasfreq=0;}
    
    }
    
    
	//ZOUT0(0)=m_currfreq;
	//ZOUT0(1)=m_hasfreq;
    

}	

