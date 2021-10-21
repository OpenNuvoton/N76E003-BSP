#define TRUE       1
#define FALSE     0

#define Timer0Out_Counter    800

#define CMD_CONNECT          0xAE
#define CMD_SYNC_PACKNO      0xA4
#define CMD_GET_FWVER        0xA6
#define FW_VERSION           0x28
#define CMD_RUN_APROM        0xAB
#define CMD_GET_DEVICEID     0xB1
#define CMD_ERASE_ALL        0xA3
#define CMD_READ_CONFIG      0xA2
#define CMD_UPDATE_CONFIG    0xA1
#define CMD_UPDATE_APROM     0xA0
#define PAGE_ERASE_AP        0x22
#define BYTE_READ_AP         0x00
#define BYTE_PROGRAM_AP      0x21
#define BYTE_READ_ID         0x0C
#define PAGE_ERASE_CONFIG    0xE2
#define BYTE_READ_CONFIG     0xC0
#define BYTE_PROGRAM_CONFIG  0xE1
#define READ_UID             0x04
#define PAGE_SIZE            128
#define APROM_SIZE           16*1024

 
extern  xdata volatile uint8_t uart_rcvbuf[64]; 
extern  xdata volatile uint8_t uart_txbuf[64];
extern data volatile uint8_t bufhead;
extern  data volatile uint16_t flash_address; 
extern  data volatile uint16_t AP_size;
extern  data volatile uint8_t g_timer1Counter;
extern  data volatile uint8_t count; 
extern  data volatile uint16_t g_timer0Counter;
extern  data volatile uint32_t g_checksum;
extern  data volatile uint32_t g_totalchecksum;
extern  bit volatile bUartDataReady;
extern  bit volatile g_timer0Over;
extern  bit volatile g_timer1Over;
extern  bit volatile g_programflag;

extern unsigned char PID_highB,PID_lowB,DID_highB,DID_lowB,CONF0,CONF1,CONF2,CONF4;
extern unsigned char recv_CONF0,recv_CONF1,recv_CONF2,recv_CONF4;

void TM0_ini(void);
void MODIFY_HIRC_16588(void);
void MODIFY_HIRC_16(void);
void Package_checksum(void);
void Send_64byte_To_UART0(void);
void UART0_ini_115200(void);
void MODIFY_HIRC_24(void);
void MODIFY_HIRC_16(void);
void READ_ID(void);
void READ_CONFIG(void);