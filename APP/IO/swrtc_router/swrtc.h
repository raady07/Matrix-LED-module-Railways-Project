
#ifndef  SWRTC_H
#define  SWRTC_H

typedef struct _SWRTCTime_ 
{
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
}__SWRTCTime;
	
void Compute_swrtc(void);
#endif



