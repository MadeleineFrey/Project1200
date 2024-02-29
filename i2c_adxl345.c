#include <pic32mx.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "adxl.h"
#include "mipslab.h"  

int ADXL_VALUE=0; //This is the signal from the accelerometer

/* Wait for I2C bus to become idle. Checks if any of the ACKEN, RCEN, PEN, RSEN, SEN bits are set to 1 and if the TRSTAT bit are set to 1.
If any of the bits are set to 1, an I2C operation is in progress. If the TRSTAT bit is set a tranmit operation is in progress. */
void i2c_wait() {
	int count = 0;
    timer4_conf(0.1);
	timer4Start();

    while (I2C1CON & 0x1F || I2C1STAT & (1 << 14))
	{
		if (IFS(0) & TMR4_FLAG)
		{
				IFS(0) &= ~TMR4_FLAG;
				if (count%10 == 0)
				{
					display_clear();
					display_string(0, "I2C ERROR");
					wait_2();
					i2c_init();
					displayMenu();
				}
		}
	}
	
}

/* First, the functions checks so no other operation is on going on the i2c bus. Then it clears the ACKDT bit which is preparing the master to send an acknowledge bit to the slave.
Then it sets the ACKEN bit. This sends the previously set acknowledge bit to the slave. If the five lsb in the I2CCON register isn't clear
when sendning an acknowledgement, the acknowledgement isn't sent. */
void i2c_ack() {
	i2c_wait();
	I2C1CONCLR = 1 << 5; //ACKDT = 0
	I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* First, the functions checks so no other operation is on going on the i2c bus. Then it sets the ACKDT bit which is preparing the master to send an not-acknowledge bit to the slave.
Then it sets the ACKEN bit. This sends the previously set not-acknowledge bit to the slave. If the five lsb in the I2CCON register isn't clear
when sendning an acknowledgement, the acknowledgement isn't sent.*/
void i2c_nack() {
	i2c_wait();
	I2C1CONSET = 1 << 5; //ACKDT = 1
	I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* Leaves the SCL signal high and pulls the SDA signal low. By that the recieving device knows a message is about to start. 
The bit generates a start condition and it is wrong to say that it is cleared but after address is sent it is "cleared". If the five lsb in the I2CCON register isn't clear
when sendning an acknowledgement, the acknowledgement isn't sent.*/
void i2c_start() {
	i2c_wait();
	I2C1CONSET = 1 << 0; //SEN
	i2c_wait();
}

/* This is a restart condition similar to the start condition. If the five lsb in the I2CCON register isn't clear
when sendning an acknowledgement, the acknowledgement isn't sent. */
void i2c_restart() {
	i2c_wait();
	I2C1CONSET = 1 << 1; //RSEN
	i2c_wait();
}

/* The stop condition does the opposite to a start condition. If the five lsb in the I2CCON register isn't clear
when sendning an acknowledgement, the acknowledgement isn't sent.*/
void i2c_stop() {
	i2c_wait();
	I2C1CONSET = 1 << 2; //PEN
	i2c_wait();
}


/*Written by Erica*/
/*Since i2c doesn't use negative values it's most safe to use unsigned integers.
First the I2C1CON register is cleared. After that we set the Baud rate to I2C standard mode (100 kHz). A table of different hexadecimal values to set the baud rate is found in the FRS
After that the STAT register is cleared. Then it starts the I2C communication by setting the on-bit.  The recieve buffer is cleared
for good practise. That operation may not be optimized by the compiler because it has a side effect. When the software reads from
the RCV register the I2CSTAT RBF bit is cleared. */
void i2c_init(void){
    volatile uint8_t temp; 
	I2C1CON = 0x0;
	I2C1BRG = 0x0C2; //PBCLK = 40MHz
	I2C1STAT = 0x0;
	I2C1CONSET = 1 << 15; // ON = 1
	temp = I2C1RCV; //Clear receive buffer for good practise.

    }



/* First, check if the i2c bus is used by another i2c operation, then it send the data to the transmitt register in the i2c buss. After that tha acknowledge status is returned.
If the reciever sends an acknowledgebit the ACKSTAT bit is 0, of it is no acknowledged the ACKSTAT bit is 1.*/
bool i2c_tx(uint8_t data) {
	i2c_wait();
	I2C1TRN = data; //The data that's transmitted
	i2c_wait();	//A very important step to make sure that the transmitt operation is finished before crecieving the acknowledgement bit
	return !(I2C1STAT & (1 << 15));
}

/* First, the recieve enable bit is set. After that the recieve register from the I2C communication is returned, */
uint8_t i2c_rx() {
	i2c_wait();
	I2C1CONSET = 1 << 3; //RCEN = 1
	i2c_wait();
	return I2C1RCV;
}



/*This function initializes the ADXL345 accelerometer*/
void adxl_init (void){
	/*First the ADXL345 address is sent followed by a write bit (0)
	Set enable measure by setting the measure enable bit
	The while loop wait until the acknoledgebit is recieved.*/
	do {
		i2c_start();
	} while(!i2c_tx(ADXL345_ADDRESS << 1));
	while (!i2c_tx(POWER_CTL));
    // measurer enable bit in the power control register
	while (!i2c_tx(1<<3));
    i2c_stop();

	/*Send write information to the data format register about the resolution*/
	do {
		i2c_start();
	} while(!i2c_tx(ADXL345_ADDRESS << 1));
	while (!i2c_tx(DATA_FORMAT));
	while (!i2c_tx(0x0)); //+-2 g resolution 10 bit mode
    i2c_stop();

/*Set the baud rate*/
	do {
		i2c_start();
	} while(!i2c_tx(ADXL345_ADDRESS << 1));
	while (!i2c_tx(BW_RATE));
	while (!i2c_tx(0x0A)); // 100 Hz datarate
    i2c_stop();

/*Set offset for the y-axis*/
	do {
		i2c_start();
	} while(!i2c_tx(ADXL345_ADDRESS << 1));
	while (!i2c_tx(OFSY));
	while (!i2c_tx(10)); 
    i2c_stop();

/*Set offset for the x-axis*/
	do {
		i2c_start();
	} while(!i2c_tx(ADXL345_ADDRESS << 1));
	while (!i2c_tx(OFSX));
	while (!i2c_tx(0x10));  
    i2c_stop();

/*Set offset for the z-axis*/
	do {
		i2c_start();
	} while(!i2c_tx(ADXL345_ADDRESS << 1));
	while (!i2c_tx(OFSZ));
	while (!i2c_tx(0x6)); // 
    i2c_stop();
}

/*This function recieves information from the accelerometer*/
void yaxis_data (int * data){

/*First, a write condition is sent to the accelerometer telling it that we want to access the DATAY0 register*/
		do {
			i2c_start();
		} while(!i2c_tx(ADXL345_ADDRESS << 1)); // 0 write mode
		while (!i2c_tx(DATAY0));

/*Then, a read condition is sent to the accelerometer*/
		do {
			i2c_start();
		} while(!i2c_tx((ADXL345_ADDRESS << 1) | 1)); //1 read mode
		
/*Now, we can start recieving data from the DATAY0 register*/
		*data = i2c_rx(); //LSB
/*By sending an acknowledgebit and then sending a recieve message again we automatically can read from the next register, in this case DATAY1*/
		i2c_ack();
		*data |= i2c_rx() << 8; //MSB

/*A nack condition is sent to the slave device to tell it that we don't expect more data. After that we stop the transmission to make the I2C bus idle*/
		i2c_nack();
		i2c_stop();
/*Because our desired resolution is +-2g we need to divide it with 256 according to the data sheet.*/
		*data = *data/128;	//resolution +- 2g

}

void xaxis_data (int * data){


		do {
			i2c_start();
		} while(!i2c_tx(ADXL345_ADDRESS << 1)); 
		while (!i2c_tx(DATAX0));

		do {
			i2c_start();
		} while(!i2c_tx((ADXL345_ADDRESS << 1) | 1)); 
		

		*data = i2c_rx(); //LSB
		i2c_ack();
		*data |= i2c_rx() << 8; //MSB

		i2c_nack();
		i2c_stop();
		*data = *data/256;	

}

void zaxis_data (int * data){

		do {
			i2c_start();
		} while(!i2c_tx(ADXL345_ADDRESS << 1)); 
		while (i2c_tx(DATAZ0));

		do {
			i2c_start();
		} while(!i2c_tx((ADXL345_ADDRESS << 1) | 1)); 
		
		*data = i2c_rx(); 
		i2c_ack();
		*data |= i2c_rx() << 8; 

		i2c_nack();
		i2c_stop();
		*data = *data/256;

}

/*This function recieves information from the accelerometer*/
int adxl_rand (int timer){
	int data;

/*First, a write condition is sent to the accelerometer telling it that we want to access the DATAY0 register*/
		do {
			i2c_start();
		} while(!i2c_tx(ADXL345_ADDRESS << 1)); // 0 write mode
		while (!i2c_tx(DATAY0));

/*Then, a read condition is sent to the accelerometer*/
		do {
			i2c_start();
		} while(!i2c_tx((ADXL345_ADDRESS << 1) | 1)); //1 read mode
		
/*Now, we can start recieving data from the DATAY0 register*/
		data = i2c_rx(); //LSB
/*By sending an acknowledgebit and then sending a recieve message again we automatically can read from the next register, in this case DATAY1*/
		i2c_ack();
		data |= i2c_rx() << 8; //MSB

/*A nack condition is sent to the slave device to tell it that we don't expect more data. After that we stop the transmission to make the I2C bus idle*/
		i2c_nack();
		i2c_stop();
		
		data += timer;
		data &= 0x0F;	//resolution +- 2g
	return data;
}







