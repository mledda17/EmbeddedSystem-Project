/**
 *
 * Final Lab Advanced Embedded Systems
 * Student: Marco Ledda
 * ID: 70/90/00471
 *
 * **/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xuartps.h"
#include <stdbool.h>
#include <stdlib.h>

#define DIM 50

/**
 *
 * Reading function
 * Basically this function uses the XUartPs_RecvByte() in order
 * to access the UART FIFO and read the content inside it.
 * It reads only if there is content inside.
 *
 * **/
u8 read_from_uart(u32 baseAddress)
{
	return XUartPs_RecvByte(baseAddress);
}

/**
 *
 * Writing function
 * This function uses the XUartPs_SendByte() to access and write
 * into the UART FIFO. It writes only if the FIFO is not full.
 *
 * **/

void write_to_uart(u32 baseAddress, u8 data)
{
	XUartPs_SendByte(baseAddress, data);
}

/* ImageData is a struct type containing sizes of the image
 * and the content of pixels described as u8
 */
typedef struct{
	int height;
	int width;
	u8* image;
} ImageData;

u8* negative(u32 baseAddress, ImageData* img_data)
{
	int i = 0;
	// Total pixels of the image is the product among height, width and 3 (RGB)
	int total_pixels = img_data->height * img_data->width * 3;

	u8* neg_image = (u8*)malloc(total_pixels * sizeof(u8*));

	/* In this block I process the pixels of the original image.
	 * The negative image will be made up of pixels computed as 255 - the value of the pixel
	 * of the original image
	 */
	for(i=0; i < total_pixels; i++){
		neg_image[i] = 255 - img_data->image[i];
	}

	for(i=0; i < total_pixels; i++){
		write_to_uart(baseAddress, neg_image[i]);
	}

	return neg_image;
}

u8* receive_image(u32 baseAddress)
{
	u8 data;
	u8 height[DIM];
	u8 width[DIM];
	int flag = 1;
	int i = 0;
	int j = 0;
	int k = 0;

	ImageData* img_data = (ImageData*)malloc(sizeof(ImageData));

	/* This iteration is done in order to read the header of the PPM file.
	 * Basically we read the first 3 lines, focusing on the second one which
	 * contains height and width.
	 *
	 * How the iteration is done: at i=1 data contains the lines where we
	 * find height and width. Every time we iterate a character, we save it
	 * into an array specially oversized. When we find a space (0x20) it means
	 * that the iteration over height is finished and we start with the
	 * iteration over width.
	 *
	 * We iterate until we meet a \n.
	 */


	for(i = 0; i < 3; i++){
		do{
			data = read_from_uart(baseAddress);
			write_to_uart(baseAddress, data);
			if(data != 0xA){
				if(i == 1 && data != 0x20 && flag == 1){
					height[j] = data;
					j++;
				}else if(i == 1 && data == 0x20){
					flag = 0;
				}else if(i == 1){
					width[k] = data;
					k++;
				}
			}
		}while(data != 0xA);
	}

	/**
	 * Conversion and size management of height and width
	 *
	 * We initialize the fields height and width of the struct to zero.
	 * Then, we iterate over height and width array characters.
	 *
	 * j and k are respectively the size of array height and array width.
	 *
	 * Suppose that we have dimension with more than 2 digits: eg. 128x128
	 *
	 * In this case height[] and width[] have dimension 3. We iterate over
	 * each character and we subtract 48, which is the code corresponding to '0'.
	 *
	 * For 128:
	 * Thus j=3. For i=0 -> img_data->height = 0*10 + (49-48) = 1
	 * For i=1 -> img_data->height = 1*10 + (50-48) = 12
	 * For i=2 -> img_data->height = 12*10 + (56-48) = 128
	 *
	 * And we stop. We obtain the corresponding integer.
	 * **/

	// Convert height to integer
	img_data->height = 0;
	for (int h = 0; h < j; h++) {
		img_data->height = img_data->height * 10 + (height[h] - '0');
	}

	// Convert width to integer
	img_data->width = 0;
	for (int w = 0; w < k; w++) {
		img_data->width = img_data->width * 10 + (width[w] - '0');
	}

	int total_pixels = img_data->height * img_data->width * 3;

	img_data->image = (u8*)malloc(total_pixels * sizeof(u8));

	// We read from the uart and we upload each pixel into the image[] field of the struct
	for(i=0; i < total_pixels; i++){
		img_data->image[i] = read_from_uart(baseAddress);
	}

	u8* neg_img;

	// We call the negative, passing the img_data to process
	neg_img = negative(baseAddress, img_data);

	return neg_img;
}



int main()
{
    init_platform();
    XUartPs Uart_1_PS;
    u16 DeviceId_1= XPAR_PS7_UART_1_DEVICE_ID;
    int Status_1;
    XUartPs_Config *Config_1;
    Config_1 = XUartPs_LookupConfig(DeviceId_1);

    if (NULL == Config_1) {
      return XST_FAILURE;
    }

    Status_1 = XUartPs_CfgInitialize(&Uart_1_PS, Config_1, Config_1->BaseAddress);

    if (Status_1 != XST_SUCCESS) {
      return XST_FAILURE;
    }

    u32 BaudRate = (u32)115200;
    Status_1 = XUartPs_SetBaudRate(&Uart_1_PS, BaudRate);

    if (Status_1 != (s32)XST_SUCCESS) {
      return XST_FAILURE;
    }

    // We call the receive_image() function passing the BaseAddress
    // We clear the memory using free() in order to manage better the memory
    free(receive_image(Config_1->BaseAddress));

    cleanup_platform();
    return 0;
}


