/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int dx[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

int ring(int m, int n) {
	return (m + n) % (m);
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	Color *newState = (Color*)malloc(sizeof(Color));
	int isAliveR, isAliveG, isAliveB;
	int isAliveCountR, isAliveCountG, isAliveCountB;
	int newRow, newCol;
	int idxR, idxG, idxB;

	isAliveR = ((*(image->image + row * image->cols + col))->R == 255);
	isAliveG = ((*(image->image + row * image->cols + col))->G == 255);
	isAliveB = ((*(image->image + row * image->cols + col))->B == 255);

	for (int i = 0; i < 8; i++) {
		newRow = ring(image->rows, row + dx[i]);
		newCol = ring(image->cols, col + dy[i]);
		if ((*(image->image + newRow * image->cols + newCol))->R == 255) {
			isAliveCountR +=1;
		}
		if ((*(image->image + newRow * image->cols + newCol))->G == 255) {
                        isAliveCountG +=1;
                }
		if ((*(image->image + newRow * image->cols + newCol))->B == 255) {
                        isAliveCountB +=1;
                }
	}

	idxR = isAliveR * 9 + isAliveCountR;
	idxG = isAliveG * 9 + isAliveCountG;
	idxB = isAliveB * 9 + isAliveCountB;

	if (rule & 1<<(idxR)) {
		newState->R = 255;
	}
	else {
		newState->R = 0;
	}

	if (rule & 1<<(idxG)) {
                newState->G = 255;
        }
        else {
                newState->G = 0;
        }

	if (rule & 1<<(idxB)) {
                newState->B = 255;
        }
        else {
                newState->B = 0;
        }

	
	
	return newState;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image *newImg = (Image*)malloc(sizeof(Image));
	newImg->rows = image->rows;
	newImg->cols = image->cols;
	newImg->image = (Color**)malloc(sizeof(Color*) * (newImg->rows) * (newImg->cols));
       	Color **p = newImg->image;
	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			*p = evaluateOneCell(image, i, j, rule);
			p++;
		}
	}	
	return newImg;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
		return 1;
	}
	Image* img = readData(argv[1]);
	uint32_t rule = strtol(argv[2], NULL, 16);
        Image* nextImg = life(img, rule);
	writeData(nextImg);
	freeImage(img);
	freeImage(nextImg);
	return 0;
}
