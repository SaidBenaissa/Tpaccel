/*

 * compile with command (don't forget to source the env.sh of your development folder!):
 		arm-angstrom-linux-gnueabi-gcc kh4test.c -o khepera4_test -I $INCPATH -L $LIBPATH -lkhepera 
*/
#include <khepera/khepera.h>
#include <signal.h>

//#define DEBUG 1
static knet_dev_t * dsPic; // robot pic microcontroller access
int maxsp,accinc,accdiv,minspacc, minspdec; // for speed profile
static int quitReq = 0; // quit variable for loop
	
/*!
 * Main
 */
int main(int argc , char * argv[]) 
{ 
#define IR_BAR_LEN 15 	// display bar length for IR sensor
#define US_BAR_LEN 23 	// display bar length for US sensor
#define ACGY_BAR_LEN 30 // display bar length for Accel/gyro sensor
#define MAX_US_DISTANCE 250.0 // max distance US
#define MAX_G 2 		// max acceleration in g

// convert US value to text comment
#define US_VAL(val) ((val)==KH4_US_DISABLED_SENSOR ? "Not activated" : ((val)==KH4_US_NO_OBJECT_IN_RANGE ? "No object in range" : ((val)==KH4_US_OBJECT_NEAR ? "Object at less than 25cm" : "Object in range 25..250cm")))

  double fpos,dval,dmean;
  long lpos,rpos;
  char Buffer[100],bar[12][64],revision,version;
  int i,n,type_of_test=0,sl,sr,pl,pr;
  short index, value,sensors[12],usvalues[5];
  char c;
  long motspeed;
  char line[80],l[9];
  int kp,ki,kd;
  int pmarg;
  
  // initiate libkhepera and robot access
  if ( kh4_init(argc ,argv)!=0)
  {
  	printf("\nERROR: could not initiate the libkhepera!\n\n");
  	return -1;
  }	

  /* open robot socket and store the handle in their respective pointers */
  dsPic  = knet_open( "Khepera4:dsPic" , KNET_BUS_I2C , 0 , NULL );

	if ( dsPic==NULL)
  {
  	printf("\nERROR: could not initiate communication with Kh4 dsPic\n\n");
  	return -2;
  }	

  /* initialize the motors controlers*/
   /* tuned parameters */
  /*pmarg=20;
  kh4_SetPositionMargin(pmarg,dsPic ); 				// position control margin
  kp=10;
  ki=5;
  kd=1;
  kh4_ConfigurePID( kp , ki , kd,dsPic  ); 		// configure P,I,D
  */
  accinc=3;//3;
  accdiv=0;
  minspacc=20;
  minspdec=1;
  maxsp=400;
// configure acceleration slope
  kh4_SetSpeedProfile(accinc,accdiv,minspacc, minspdec,maxsp,dsPic ); // Acceleration increment ,  Acceleration divider, Minimum speed acc, Minimum speed dec, maximum speed
 kh4_SetMode( kh4RegIdle,dsPic);  				     // Put in idle mode (no control)

// get accel sensor
int j=0;
for (j=0;j<10;j++)
{					
	kh4_measure_acc((char *)Buffer, dsPic);
	printf("\nAcceleration sensor [g]\n       new data                                            old data  average  [g]: -2     -1      0      1      2\nacc  X: ");
	dmean=0;
	for (i=0;i<10;i++)
		{
			dval=((short)(Buffer[i*2] | Buffer[i*2+1]<<8)>>4)/1000.0;
			printf("%5.2f ",dval);
			dmean+=dval;                                                                                       
		}
		
	dval=dmean/10.0;
	
	printf(" %5.2f",dval);   
						

}
 	return 0;
}

