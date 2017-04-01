
/*******************************************************************************
* name: Connectivity
*
* preset name: Sumotoy_Monochrome
* data block size: 32 bit(s), uint32_t
* RLE compression enabled: no
* conversion type: Monochrome, Edge 128
* bits per pixel: 1
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: no
* ------------------------------------------------------------------------------
* Created by a custom template of LCD-Image-Converter for .s.u.m.o.t.o.y.
* Image template version: 1.0
* Note: 
* ------------------------------------------------------------------------------
*******************************************************************************/


#include <stdint.h>


/*
static const uint32_t image_data_Connectivity[24] __PRGMTAG_  = {
    0x007e0000, 
    0x03ff8000, 
    0x07ffe000, 
    0x0f00f000, 
    0x1e003800, 
    0x38ff3c00, 
    0x79ff1c00, 
    0x73e38e00, 
    0x6701c600, 
    0xe738c700, 
    0xe67ce700, 
    0xe67e6700, 
    0xe67e6700, 
    0xe63c6700, 
    0xe718e700, 
    0x6718c600, 
    0x70180e00, 
    0x30181c00, 
    0x3c183c00, 
    0x1c183800, 
    0x08181000, 
    0x00180000, 
    0x00180000, 
    0x00180000
};// 32
*/
static const uint32_t image_data_Connectivity[24]  = {
	0x007e0000,
	0x03ff8000,
	0x07ffe000,
	0x0f00f000,
	0x1e003800,
	0x38ff3c00,
	0x79ff1c00,
	0x73e38e00,
	0x6701c600,
	0xe738c700,
	0xe67ce700,
	0xe67e6700,
	0xe67e6700,
	0xe63c6700,
	0xe718e700,
	0x6718c600,
	0x70180e00,
	0x30181c00,
	0x3c183c00,
	0x1c183800,
	0x08181000,
	0x00180000,
	0x00180000,
	0x00180000
};// 32
static const tIcon Connectivity = { image_data_Connectivity, 24, 24, 24};

