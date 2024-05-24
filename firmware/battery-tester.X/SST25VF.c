#include <xc.h>
#include <stdio.h>
#include "main.h"
#include "MMSP.h"
#include "settings.h"

#define		READ_ID					0X90
#define		READ_ID_ADD				0X00
#define		JEDEC_READ_ID			0X9F
#define		READ					0x03
#define		BYTE_PROGRAM			0X02
#define 	WRITE_EN				0x06
#define		WRITE_DB				0X04
#define		EBSY					0x70    // Enable SO as RY/BY# status during AAI programming
#define		DBSY					0x80    // Disable SO as RY/BY# status during AAI programming
#define		READ_REGISTER_STATUS	0x05
#define		CHIP_ERASE				0X60
#define		SRB_BUSY				0x01	//status register bit: busy		
#define		SRB_WRITEEN				0x02
#define		WRITE_STAT_REG_EN		0x50
#define     SECTOR_ERASE            0x20

#define     AAI_WORD_PROGRAM        0xAD

#define ENABLE_SST25        LATFbits.LF7=0
#define DISABLE_SST25       LATFbits.LF7=1

/*
*    @brief Checks SST25 flash memory chip busy status.
*/

void check_busy(void)
{
    uint8_t status_register_byte;

    ENABLE_SST25;

    while (1)
    {
        SPI_Exchange(READ_REGISTER_STATUS);
        status_register_byte = SPI_Exchange(0);

        if ((status_register_byte & SRB_BUSY) == 0)
        {
            DISABLE_SST25;
            break;
        }
    }
}
/**
*  @brief Checks if SST25 flash memory chip write enable status.
*/
void check_write_en(void)
{
    ENABLE_SST25;

    uint8_t status_register_byte;

    while (1)
    {
        SPI_Exchange(READ_REGISTER_STATUS);
        status_register_byte = SPI_Exchange(0xFF);
        if ((status_register_byte & SRB_WRITEEN) >> 1 == 1)
        {
            DISABLE_SST25;
            break;
        }
    }
}
	
/**
*   @brief Reads manufacturer and device ID from SST25 flash memory chip.
*/

void read_id(void)
{
    ENABLE_SST25;

    uint8_t tab[2];

    SPI_Exchange(READ_ID);
    SPI_Exchange(0x00);
    SPI_Exchange(0x00);
    SPI_Exchange(READ_ID_ADD);
    tab[0] = SPI_Exchange(0xFF);
    tab[1] = SPI_Exchange(0xFF);

    printf("Manufacturer ID: %x \r\nDevice ID: %x \r\n", tab[0], tab[1]);

    DISABLE_SST25;
}
/**
* @brief Reads JEDEC manufacturer ID, device ID, and memory capacity from SST25 flash memory chip.
*/
void read_id_jedec(void)
{
	ENABLE_SST25;

    uint8_t tab[3];
    
	SPI_Exchange(JEDEC_READ_ID);
    tab[0]=SPI_Exchange(0xFF);
    tab[1]=SPI_Exchange(0xFF);
    tab[2]=SPI_Exchange(0xFF);
    
	printf("Manufacurer ID: %x \r\nDevice ID: %x \r\nMemory Capacity: %x \r\n", tab[0], tab[1], tab[2]);
    
	DISABLE_SST25;
}
/**
 * @brief Reads bytes from SST25 flash memory chip.
 * 
 * @param add: The starting address to read from.
 * @param data: Pointer to the buffer to store read data.
 * @param lenght: Number of bytes to read.
 */
void read_bytes(uint32_t add, uint8_t *data, uint8_t lenght)
{
    uint8_t i;

    ENABLE_SST25;
    SPI_Exchange(READ);
    SPI_Exchange(((add & 0xFFFFFF) >> 16));
    SPI_Exchange(((add & 0x00FFFF) >> 8));
    SPI_Exchange(add & 0x0000FF);
    for (i = 0; i < lenght; i++)
    {
        *(data + i) = SPI_Exchange(0);
    }

    DISABLE_SST25;
}

/**
 * @brief Initializes and enables write operation on SST25 flash memory chip.
 */
void sst25vf_init_enable_write(void)
{
    ENABLE_SST25;
    SPI_Exchange(WRITE_STAT_REG_EN);
    DISABLE_SST25;

    SPI_Exchange(0x00);

    ENABLE_SST25;
    SPI_Exchange(0x01);
    SPI_Exchange(0x00);
    DISABLE_SST25;
}

/**
 * @brief Writes a byte to SST25 flash memory chip.
 * 
 * @param add: The address to write to.
 * @param data: The byte of data to write.
 */
void write_byte(uint32_t add, uint8_t data)
{
    // Enable end checking WRITE bit------------------------
    ENABLE_SST25;
    SPI_Exchange(WRITE_EN);
    DISABLE_SST25;

    check_write_en();

    //-----------------------------------------------------
    // writing data to FLASH--------------------------------
    ENABLE_SST25;
    SPI_Exchange(BYTE_PROGRAM);
    SPI_Exchange(((add & 0xFFFFFF) >> 16));
    SPI_Exchange(((add & 0x00FFFF) >> 8));
    SPI_Exchange(add & 0x0000FF);
    SPI_Exchange(data);

    DISABLE_SST25;
    //-----------------------------------------------------
    // Waiting to finish writing - Checking Busy flag

    check_busy();

    ENABLE_SST25;
    SPI_Exchange(WRITE_DB);
    DISABLE_SST25;
}

/**
 * @brief Writes a table of bytes with auto address increment to SST25 flash memory chip.
 * 
 * @param add: The starting address to write to.
 * @param data: Pointer to the buffer containing data to write.
 * @param length: The number of bytes to write.
 */
void write_byte_table_auto_address_increment(uint32_t add, uint8_t *data, uint8_t length)
{
    // Enable end checking WRITE bit------------------------
    ENABLE_SST25;
    SPI_Exchange(DBSY);
    DISABLE_SST25;

    ENABLE_SST25;
    SPI_Exchange(WRITE_EN);
    DISABLE_SST25;

    check_write_en();

    ENABLE_SST25;
    SPI_Exchange(AAI_WORD_PROGRAM);
    SPI_Exchange(((add & 0xFFFFFF) >> 16));
    SPI_Exchange(((add & 0x00FFFF) >> 8));
    SPI_Exchange(add & 0x0000FF);
    SPI_Exchange(*(data));
    SPI_Exchange(*(data + 1));
    DISABLE_SST25;

    check_busy();

    for (uint8_t i = 2; i < length; i = i + 2)
    {
        // writing data to FLASH
        ENABLE_SST25;
        SPI_Exchange(AAI_WORD_PROGRAM);
        SPI_Exchange(*(data + i));
        SPI_Exchange(*(data + i + 1));
        DISABLE_SST25;

        check_busy();
    }

    ENABLE_SST25;
    SPI_Exchange(WRITE_DB);
    DISABLE_SST25;
}

/**
 * @brief Erases the entire chip of SST25 flash memory.
 */
void chip_erase(void)
{
    ENABLE_SST25;
    SPI_Exchange(WRITE_EN);
    DISABLE_SST25;

    check_write_en();

    ENABLE_SST25;
    SPI_Exchange(CHIP_ERASE);
    DISABLE_SST25;

    check_busy();
}

/**
 * @brief Erases a sector of SST25 flash memory.
 * 
 * @param add: The address within the sector to erase.
 */
void sector_erase(uint32_t add)
{
    ENABLE_SST25;
    SPI_Exchange(WRITE_EN);
    DISABLE_SST25;

    check_write_en();

    ENABLE_SST25;
    SPI_Exchange(SECTOR_ERASE);
    SPI_Exchange(((add & 0xFFFFFF) >> 16));
    SPI_Exchange(((add & 0x00FFFF) >> 8));
    SPI_Exchange(add & 0x0000FF);
    DISABLE_SST25;

    check_busy();
}

/**
 * @brief Erases a block of SST25 flash memory.
 * 
 * @param add: The starting address of the block to erase.
 * @param block_type: The type of block to erase can be 32kB or 64kB
 */
void block_erase(uint32_t add, uint8_t block_type)
{
    ENABLE_SST25;
    SPI_Exchange(WRITE_EN);
    DISABLE_SST25;

    check_write_en();

    ENABLE_SST25;
    SPI_Exchange(block_type);
    SPI_Exchange(((add & 0xFFFFFF) >> 16));
    SPI_Exchange(((add & 0x00FFFF) >> 8));
    SPI_Exchange(add & 0x0000FF);
    DISABLE_SST25;

    check_busy();
}
