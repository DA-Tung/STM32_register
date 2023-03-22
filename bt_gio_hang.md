# L-thuy-t-C

#include <stdio.h>
#include <stdint.h>
typedef enum{
    AO          = 1 << 0,   //0b00000001
    QUAN        = 1 << 1,   //0b00000010
    VAY         = 1 << 2,   //0b00000100
    DAM         = 1 << 3,   //0b00001000
    NHAN        = 1 << 4,   //0b00010000
    VONG_TAY    = 1 << 5,   //0b00100000
    GIAY        = 1 << 6,   //0b01000000
    TUI         = 1 << 7    //0b10000000
}DoDungCaNhan;

void Them_Vao_Gio_Hang(uint8_t *gio_hang, DoDungCaNhan ten_do_dung)
{
    *gio_hang |= ten_do_dung;
}

void Xoa_Khoi_Gio_Hang(uint8_t *gio_hang, DoDungCaNhan ten_do_dung)
{
    *gio_hang &= ~ten_do_dung;
}

void Kiem_Tra_San_Pham(uint8_t gio_hang, DoDungCaNhan ten_do_dung)
{
    if(ten_do_dung & (1 << 0))
    {
        if(gio_hang & (1 << 0))
        {
            printf("Gio hang co : AO\n");
        }
        else
        {
            printf("Gio hang khong co : AO\n");  
        }
    }
    
    if(ten_do_dung & (1 << 1))
    {
        if(gio_hang & (1 << 1))
        {
            printf("Gio hang co : QUAN\n");
        }
        else
        {
            printf("Gio hang khong co : QUAN\n");  
        }        
    }   

    if(ten_do_dung & (1 << 2))
    {
        if(gio_hang & (1 << 2))
        {
            printf("Gio hang co : VAY\n");
        }
        else
        {
            printf("Gio hang khong co : VAY\n");  
        }        
    }
    
    if(ten_do_dung & (1 << 3))
    {
        if(gio_hang & (1 << 3))
        {
            printf("Gio hang co : DAM\n");
        }
        else
        {
            printf("Gio hang khong co : DAM\n");  
        }        
    }
    
    if(ten_do_dung & (1 << 4))
    {
        if(gio_hang & (1 << 4))
        {
            printf("Gio hang co : NHAN\n");
        }
        else
        {
            printf("Gio hang khong co : NHAN\n");  
        }        
    }
    
    if(ten_do_dung & (1 << 5))
    {
        if(gio_hang & (1 << 5))
        {
            printf("Gio hang co : VONG_TAY\n");
        }
        else
        {
            printf("Gio hang khong co : VONG_TAY\n");  
        }        
    }
    
    if(ten_do_dung & (1 << 6))
    {
        if(gio_hang & (1 << 6))
        {
            printf("Gio hang co : GIAY\n");
        }
        else
        {
            printf("Gio hang khong co : GIAY\n");  
        }        
    }
    
    if(ten_do_dung & (1 << 7))
    {
        if(gio_hang & (1 << 7))
        {
            printf("Gio hang co : TUI\n");
        }        
        else
        {
            printf("Gio hang khong co : TUI\n");  
        }        
    }    
}

void Hien_Thi_Gio_Hang(uint8_t gio_hang)
{
    //DoDungCaNhan do_dung;
    
    printf("Hien thi gio hang : ");
    
    if(gio_hang & (1 << 0))
    {
        printf("AO ");
    }
    
    if(gio_hang & (1 << 1))
    {
        printf("QUAN ");
    }  
    
    if(gio_hang & (1 << 2))
    {
        printf("VAY ");
    }
    
    if(gio_hang & (1 << 3))
    {
        printf("DAM ");
    } 
    
    if(gio_hang & (1 << 4))
    {
        printf("NHAN ");
    }
    
    if(gio_hang & (1 << 5))
    {
        printf("VONG_TAY ");
    }
    
    if(gio_hang & (1 << 6))
    {
        printf("GIAY ");
    }
    
    if(gio_hang & (1 << 7))
    {
        printf("TUI ");
    }  
    
    printf("\n");
}

void HIen_Thi(DoDungCaNhan ten_do_dung)
{

}

int main(int argc, char const *argv[])
{
    uint8_t GIO_HANG;
    
    Them_Vao_Gio_Hang(&GIO_HANG, AO|QUAN|VAY|DAM|NHAN);
    
    Hien_Thi_Gio_Hang(GIO_HANG);
    
    Xoa_Khoi_Gio_Hang(&GIO_HANG, AO|VAY);

    Kiem_Tra_San_Pham(GIO_HANG, AO|NHAN|VAY);
 
    Hien_Thi_Gio_Hang(GIO_HANG);    

    return 0;
}


