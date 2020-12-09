#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <process.h>
#include <conio.h>
#include <wchar.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include<winsock.h>
#include "resource.h"
const int bulletNum=30,flyerNum=5,bompNum=5,birdNum=3,snitchNum=1;
using namespace Gdiplus;
char* getLocapIpAddress() ;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void shooting( void *hwnd ) ;
void udpReceiver(void *hwnd) ;
HINSTANCE hInstance ;
/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";
int direction = 0 ;
int shootStart0=0;
int shootIng1=0,shootIng2=0,shootIng3=0;
int gameStart=0;
int sound=0;
int restart0=0;
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    GdiplusStartupInput	 gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup (&gdiplusToken, &gdiplusStartupInput, NULL);

    /* The Window structure */
    hInstance = hThisInstance;
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0; 

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1000,                 /* The programs width */
           700,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);
    //SetTimer(hwnd, 1, 1000, NULL);
   
    _beginthread( shooting, 0, &hwnd );
    _beginthread( udpReceiver, 0, &hwnd );
    
    mciSendString("open ..\\..\\255318__shadydave__race.mp3 type mpegvideo alias bkMusic ", NULL, 0, 0); 
    
    mciSendString("play bkMusic repeat", NULL, 0, 0);
    if(sound==1)
    mciSendString("setaudio bkMusic volume to 30", NULL, 255, 0);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    GdiplusShutdown(gdiplusToken);
    mciSendString("close bkMusic", NULL, 0, 0);
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        char str[32] ;
        case WM_KEYDOWN:{
            switch (wParam) {
                case VK_LEFT:
                    direction = -1 ;                    
                    break;
                case VK_RIGHT:
                    direction = 1 ;
                    break;
                case VK_UP:
                    //shootStart0 = 1 ;
                    break;
                case VK_DOWN:
                    direction = 0 ;
                    break;                
                case 'Q':
                    PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
                    break ;
                case ' ':
                    shootStart0 = 1 ;
                    break;    
                case 'S':
                    gameStart=1;
                    break;    
                case 'V':
                    sound=1;
                    break;    
                case 'A':
                    restart0=1;
                    break;    
                    
                case 'X':
                    //token = 5 ;
                    //sprintf(str, "X %d",token) ;
                    //MessageBox(hwnd, str , "Key Pressed", MB_OK);
                    //MessageBox(hwnd, getLocapIpAddress() , "Local IP ADDRESS", MB_OK);
                    break;
            }
            
            break;
        }
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;  //??????
		    //???handle??????handle
            HDC hdc = BeginPaint(hwnd, &ps); 
            Graphics g(hdc); //??????(??)
            
            EndPaint(hwnd, &ps);
        }
            break ;
        
        
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void shooting( void *hwnd ){
    HWND *myHwnd = (HWND *)hwnd ;
    HDC hdc = GetDC(*myHwnd );	
    	
    RECT rcClient;
    int dx = 10 ;
	int x0=0, y0=0, width=800,height=600 ;
	int picWidth, picHeight ;
    int flyerX[flyerNum],flyerY[flyerNum];
    int birdX[birdNum],birdY[birdNum];
    int snitchX[snitchNum],snitchY[snitchNum];
    int bompX[bompNum],bompY[bompNum];
	int shooterX, shooterY ;
	int imgBullet1X, imgBullet1Y ;
	int midBullet1X,midBullet1Y;
	int midflyerX[flyerNum],midflyerY[flyerNum];
	int midbirdX[birdNum],midbirdY[birdNum];
	int midsnitchX[birdNum],midsnitchY[birdNum];
	int midBulletX[bulletNum],midBulletY[bulletNum];
	
    GetClientRect(*myHwnd, &rcClient);
    width = rcClient.right ;
    height = rcClient.bottom ;
    
    //double buffering starts here	
    HDC hdcMem = CreateCompatibleDC(hdc);
    const int nMemDC = SaveDC(hdcMem);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
    SelectObject(hdcMem, hBitmap);
    Graphics g(hdcMem);  	
    //double buffering ends here
    
	//Graphics g(hdc);
	srand(time(NULL)) ;
	
	Pen pen(Color(255, 0, 50, 50)); 	
	SolidBrush darkRedBrush(Color(255,128,255,128)) ;
	Image		  imgBak (L"..\\..\\blue-sky2.jpg");//sea01b.gif
    TextureBrush  tBrush (&imgBak);
    SolidBrush bulletBrush(Color::Red) ;
	Image flyer(L"..\\..\\single-engine-fighter-animation.gif") ;
	Image bird(L"..\\..\\t013ec39a83f812d223.gif") ;
	Image snitch(L"..\\..\\snitch_by_uppun.gif") ;
	Image imgShooter(L"..\\..\\tank.gif") ;
	Image imgBullet(L"..\\..\\small_red_ball.png") ;
    Image bomp(L"..\\..\\explosion3.gif") ;
	picWidth = flyer.GetWidth() ;
	picHeight = flyer.GetHeight() ;
    
	shooterX = width/2;
	shooterY = height-100 ;
    int shootStart[bulletNum];
    int shootIng[bulletNum];
    int imgBulletX[bulletNum],imgBulletY[bulletNum];
    //int restart[n];
    int bulletWidth=10,midbulletWidth=5;
    int imgFrameCount = flyer.GetFrameCount(&FrameDimensionTime);	
    int imgFrameCount2 = bomp.GetFrameCount(&FrameDimensionTime);	
    int flyer_idx=0,bomp_idx[bompNum],bird_idx=0;
    int startExplode[bompNum];
    int startExplode0;
    int bomph=bomp.GetHeight()*0.5;
    int shootDecide;
    char str[30];
    int score;
    int snitch_idx=0;
    float time=30;
    
    for(int i=0;i<bompNum;i++){
        
        startExplode[i]=0;
    }
    for(int i=0;i<bulletNum;i++){
        
        shootIng[i]=0;
    }
    
    for(int i=0,j=0;i<flyerNum,j<birdNum;i++,j++){
        flyerX[i] = -(rand() % width/3 )-5;
        flyerY[i] = rand()%height/3 ;
        birdX[j] = -(rand() % width/3 )-10;
        birdY[j] = rand()%height/3 ;
        snitchX[i] = -(rand() % width/3 )-500;
        snitchY[i] = rand()%height/3 ;
    }
    
    do { 
        g.FillRectangle(&tBrush,x0,y0,width,height);
        g.DrawRectangle(&pen,x0,y0,width,height);
        
        wchar_t str2[10];
        //swprintf(str, L"press'S' to start the game") ;
        //MessageBox(hwnd, str , "Key Pressed", MB_OK);
        FontFamily  fontFamily(L"Times New Roman");
        Font  font(&fontFamily, 50, FontStyleRegular, UnitPixel);
        PointF pointF(280.0f, 300.0f);
        SolidBrush  solidBrush(Color(255, 255, 255, 255));
        g.DrawString(L"press'S' to start the game", -1, &font, pointF, &solidBrush);
        //double buffering start here
        BitBlt(hdc, x0, y0, width+1, height+1, hdcMem, x0, y0, SRCCOPY);
        //double buffering end here    
    }while(gameStart==0);    
    score=0;
    do{
        flyer.SelectActiveFrame(&FrameDimensionTime, flyer_idx++);
        if(flyer_idx == imgFrameCount) flyer_idx= 0;
        bird.SelectActiveFrame(&FrameDimensionTime, bird_idx++);
        if(bird_idx == imgFrameCount) bird_idx= 0;
        snitch.SelectActiveFrame(&FrameDimensionTime, snitch_idx++);
        if(snitch_idx == imgFrameCount) snitch_idx= 0;
        
        g.FillRectangle(&tBrush,x0,y0,width,height);
        g.DrawRectangle(&pen,x0,y0,width,height);
        
        for(int i=0;i<flyerNum;i++){
            if(flyerX[i]>=width){
                flyerX[i] = -(rand() % width/3 );
                flyerY[i] = rand()%height/3 ;
            }
            midflyerY[i]=flyerY[i]+flyer.GetHeight()*0.5;
    	    midflyerX[i]=flyerX[i]+flyer.GetWidth()*0.5;
            g.DrawImage(&flyer, flyerX[i], flyerY[i]) ;      
        }       
        for(int j=0;j<birdNum;j++){
            if(birdX[j]>=width){
                birdX[j] = -(rand() % width/3 )-10;
                birdY[j] = rand()%height/3 ;
            }
            midbirdY[j]=birdY[j]+bird.GetHeight()*0.5;
    	    midbirdX[j]=birdX[j]+bird.GetWidth()*0.5;
            g.DrawImage(&bird, birdX[j], birdY[j]) ;     
        }        
        for(int i=0;i<snitchNum;i++){
            if(snitchX[i]>=width){
                snitchX[i] = -(rand() % width/3 );
                snitchY[i] = rand()%height/3 ;
            }
            midsnitchY[i]=snitchY[i]+snitch.GetHeight()*0.5;
    	    midsnitchX[i]=snitchX[i]+snitch.GetWidth()*0.5;
            g.DrawImage(&snitch, snitchX[i], snitchY[i]) ;      
        }       
        g.DrawImage(&imgShooter, shooterX, shooterY) ;
        if(shootStart0!=0){
            for(int i=0;i<bulletNum;i++){            
                if(shootIng[i]==0){
                    shootStart0=0;
                    shootDecide=i;
                    if(sound==1)
                        PlaySound((LPCSTR) "..\\..\\gunShot1.wav", NULL, SND_FILENAME | SND_ASYNC); 
                   
                    break;
                }
            }
                            
            imgBulletX[shootDecide]=shooterX+imgShooter.GetWidth()*0.5-midbulletWidth;
            imgBulletY[shootDecide]=shooterY;
            shootIng[shootDecide]=1;
            sprintf(str, "%d",shootDecide) ;
            //MessageBox(*myHwnd, str , "Key Pressed", MB_OK);        
                    
            
        } 
        
        for(int i=0;i<bulletNum;i++){
            if(shootIng[i]!=0){
                for(int j=0;j<flyerNum;j++){
                    
                    midBulletY[i]=imgBulletY[i]+ midbulletWidth;
	                midBulletX[i]=imgBulletX[i]+ midbulletWidth;
                    g.DrawImage(&imgBullet, imgBulletX[i], imgBulletY[i],bulletWidth,bulletWidth) ;
                
                    if(midflyerX[j]>midBulletX[i]){
                        if((midBulletY[i]-midflyerY[j]<flyer.GetHeight()*0.5+imgBullet.GetHeight()*0.5)&&
                            (midflyerX[j]-midBulletX[i]<flyer.GetWidth()*0.5+imgBullet.GetWidth()*0.5)){
                            shootIng[i]=0;
                            flyerX[j] = -(rand() % width/3 )-5;
                            for(int k=0;k<bompNum;k++){       
                                if(startExplode[k]==0){
                                     startExplode0=k;
                                    
                                     break;
                                } 
                            }   
                            score+=100;
                            bompX[startExplode0]=midflyerX[j]-bomph;
                            bompY[startExplode0]=midflyerY[j]-bomph;
                            midBulletX[startExplode0]=-1;
                
                           
                            startExplode[startExplode0]=1;
                            bomp_idx[startExplode0]=0;
                            if(sound==1)
                                PlaySound((LPCSTR) "..\\..\\explosion1.wav", NULL, SND_FILENAME | SND_ASYNC);
                        } 
                    }
                    if(midBulletX[i]>midflyerX[j]){
                        if((midBulletY[i]-midflyerY[j]<flyer.GetHeight()*0.5+5)&&
                            (midBulletX[i]-midflyerX[j]<flyer.GetWidth()*0.5+5)){
                            shootIng[i]=0;
                            flyerX[j] = -(rand() % width/3 )-5;
                            for(int k=0;k<bompNum;k++){       
                                if(startExplode[k]==0){
                                     startExplode0=k;
                                    
                                     break;
                                } 
                            }   
                            score+=100;
                            
                            bompX[startExplode0]=midflyerX[j]-bomph;
                            bompY[startExplode0]=midflyerY[j]-bomph;
                            midBulletX[startExplode0]=-1;
                            sprintf(str, "%d %d %d",i,j,startExplode0) ;
                            //MessageBox(*myHwnd, str , "Key Pressed", MB_OK);
                            startExplode[startExplode0]=1;
                            bomp_idx[startExplode0]=0;
                            if(sound==1)
                                PlaySound((LPCSTR) "..\\..\\explosion1.wav", NULL, SND_FILENAME | SND_ASYNC);
                            }
                        }
                }
                for(int j=0;j<birdNum;j++){
                    if(midbirdX[j]>midBulletX[i]){
                        if((midBulletY[i]-midbirdY[j]<bird.GetHeight()*0.5+imgBullet.GetHeight()*0.5)&&
                            (midbirdX[j]-midBulletX[i]<bird.GetWidth()*0.5+imgBullet.GetWidth()*0.5)){
                            shootIng[i]=0;
                            birdX[j] = -(rand() % width/3 )-10;
                            for(int k=0;k<bompNum;k++){       
                                if(startExplode[k]==0){
                                     startExplode0=k;
                                    
                                     break;
                                } 
                            }   
                            score-=500;
                            bompX[startExplode0]=midflyerX[j]-bomph;
                            bompY[startExplode0]=midflyerY[j]-bomph;
                            midBulletX[startExplode0]=-1;
                
                           
                            startExplode[startExplode0]=1;
                            bomp_idx[startExplode0]=0;
                            if(sound==1)
                                PlaySound((LPCSTR) "..\\..\\birds004.wav", NULL, SND_FILENAME | SND_ASYNC);
                        } 
                    }
                    if(midBulletX[i]>midbirdX[j]){
                        if((midBulletY[i]-midbirdY[j]<bird.GetHeight()*0.5+5)&&
                            (midBulletX[i]-midbirdX[j]<bird.GetWidth()*0.5+5)){
                            shootIng[i]=0;
                            birdX[j] = -(rand() % width/3 );
                            for(int k=0;k<bompNum;k++){       
                                if(startExplode[k]==0){
                                     startExplode0=k;
                                    
                                     break;
                                } 
                            }   
                            score-=500;
                            
                            bompX[startExplode0]=midbirdX[j]-bomph;
                            bompY[startExplode0]=midbirdY[j]-bomph;
                            midBulletX[startExplode0]=-1;
                            sprintf(str, "%d %d %d",i,j,startExplode0) ;
                            //MessageBox(*myHwnd, str , "Key Pressed", MB_OK);
                            startExplode[startExplode0]=1;
                            bomp_idx[startExplode0]=0;
                            if(sound==1)
                                PlaySound((LPCSTR) "..\\..\\birds004.wav", NULL, SND_FILENAME | SND_ASYNC);
                            }
                        }    
                }
                for(int j=0;j<snitchNum;j++){
                     if(midsnitchX[j]>midBulletX[i]){
                        if((midBulletY[i]-midsnitchY[j]<snitch.GetHeight()*0.5+imgBullet.GetHeight()*0.5)&&
                            (midsnitchX[j]-midBulletX[i]<snitch.GetWidth()*0.5+imgBullet.GetWidth()*0.5)){
                            shootIng[i]=0;
                            snitchX[j] = -(rand() % width/3 )-500;
                            for(int k=0;k<bompNum;k++){       
                                if(startExplode[k]==0){
                                     startExplode0=k;
                                    
                                     break;
                                } 
                            }   
                            score+=1000;
                            bompX[startExplode0]=midsnitchX[j]-bomph;
                            bompY[startExplode0]=midsnitchY[j]-bomph;
                            midBulletX[startExplode0]=-1;
                
                           
                            startExplode[startExplode0]=1;
                            bomp_idx[startExplode0]=0;
                            if(sound==1)
                                PlaySound((LPCSTR) "..\\..\\cartoon044.wav", NULL, SND_FILENAME | SND_ASYNC);
                        } 
                    }
                    if(midBulletX[i]>midsnitchX[j]){
                        if((midBulletY[i]-midsnitchY[j]<bird.GetHeight()*0.5+5)&&
                            (midBulletX[i]-midsnitchX[j]<bird.GetWidth()*0.5+5)){
                            shootIng[i]=0;
                            snitchX[j] = -(rand() % width/3 )-500;
                            for(int k=0;k<bompNum;k++){       
                                if(startExplode[k]==0){
                                     startExplode0=k;
                                    
                                     break;
                                } 
                            }   
                            score+=1000;
                            
                            bompX[startExplode0]=midsnitchX[j]-bomph;
                            bompY[startExplode0]=midsnitchY[j]-bomph;
                            midBulletX[startExplode0]=-1;
                            sprintf(str, "%d %d %d",i,j,startExplode0) ;
                            //MessageBox(*myHwnd, str , "Key Pressed", MB_OK);
                            startExplode[startExplode0]=1;
                            bomp_idx[startExplode0]=0;
                            if(sound==1)
                                PlaySound((LPCSTR) "..\\..\\cartoon044.wav", NULL, SND_FILENAME | SND_ASYNC);
                            }
                        }   
                }    
                  //for(int j=0;j<N;j++)
                 
            } 
            //if(shootIng[i]!=0){  
                          
        }
        
        for(int i=0;i<bompNum;i++){  
            if(startExplode[i]==1){
                bomp.SelectActiveFrame(&FrameDimensionTime, bomp_idx[i]);
                g.DrawImage(&bomp, bompX[i], bompY[i]);
                bomp_idx[i]++;
                if(bomp_idx[i] >= imgFrameCount2) startExplode[i]=0;;
             } 
        }                   
        for(int i=0;i<bulletNum;i++){
            imgBulletY[i]-=10;  
            if( imgBulletY[i]<=0){
                 imgBulletY[i]=height;
                 shootIng[i]=0;
            }
        }    
        for(int j=0,k=0,w=0;j<flyerNum,k<birdNum,w<snitchNum;j++,k++,w++){
            flyerX[j]+=dx;
            birdX[k]+=dx*0.5;
            snitchX[w]+=dx*4;
            
        }     
        shooterX+=(direction*5); 
        time-=(0.02); 
        if(time<=0){
            gameStart=0; 
            wchar_t str3[10];
             swprintf(str3, L"game over \nu got score %d ",score) ;
            //MessageBox(hwnd, str , "Key Pressed", MB_OK);
            FontFamily  fontFamily(L"Times New Roman");
            Font  font(&fontFamily, 50, FontStyleRegular, UnitPixel);
            PointF pointF(300.0f, 250.0f);
            SolidBrush  solidBrush(Color(255, 255, 255, 255));
            g.DrawString(str3, -1, &font, pointF, &solidBrush);  
            /*if(restart0==1){
             time=30;
             score=0; 
             gameStart=1;  
             }*/ 
        }    
        
        if(shooterX<0||shooterX>(width-imgBullet.GetWidth()-bulletWidth))
            direction*=-1;
        wchar_t str[30];
        swprintf(str, L"score %d  time %3.0f",score,time) ;
        //MessageBox(hwnd, str , "Key Pressed", MB_OK);
        FontFamily  fontFamily(L"Times New Roman");
        Font  font(&fontFamily, 24, FontStyleRegular, UnitPixel);
        PointF pointF(30.0f, 10.0f);
        SolidBrush  solidBrush(Color(255, 0, 255, 255));
        g.DrawString(str, -1, &font, pointF, &solidBrush);

           
        //double buffering start here
        BitBlt(hdc, x0, y0, width+1, height+1, hdcMem, x0, y0, SRCCOPY);
        //double buffering end here    
        Sleep(20) ;
    }
    // Repeat while RunMutex is still taken.  drawBouncingPicture
    while ( gameStart==1 );
    //double buffering start here
    RestoreDC(hdcMem, nMemDC);
    DeleteObject(hBitmap);
    //double buffering end here   
    ReleaseDC(*myHwnd, hdc);
}


void udpReceiver(void *hwnd)
{
     //???? ??-????- ??????-???  -lws2_32
     HWND *myHwnd = (HWND *)hwnd ;
     SOCKET socket1;
     WSADATA wsaData;
     int ErrorCode;
     if(WSAStartup(MAKEWORD(2,1),&wsaData))// windows socket dll;
     {
        MessageBox(*myHwnd, "Winsock initiate failed!!" , "UDP receiver", MB_OK);
        //printf("Winsock initiate failed!!\n");
        WSACleanup();
        return ;
     }
     
     //printf("Winsock start...\n");
     struct sockaddr_in local;
     struct sockaddr_in client;
     int len = sizeof(client);
     local.sin_family = AF_INET;
     local.sin_port = htons(1234);  //the port need listened
     local.sin_addr.s_addr = INADDR_ANY; //localhost
     
     socket1 = socket(AF_INET,SOCK_DGRAM,0);
     bind(socket1,(struct sockaddr*)&local,sizeof(local));
     //MessageBox(*myHwnd, getLocapIpAddress() , "Local IP ADDRESS", MB_OK);
     while (1)
     {
        char buffer[1024] = "\0";
        //printf("waiting message from the clients...\n");
        if(recvfrom(socket1,buffer,sizeof(buffer),0,(struct sockaddr*)&client,&len) != SOCKET_ERROR)
        {
            //MessageBox(*myHwnd, buffer , "UDP receiver", MB_OK);
            if( strcmp("A",buffer) == 0) direction = -1 ;                
            if( strcmp("D",buffer) == 0) direction = 1 ; ;   
            if( strcmp("W",buffer) == 0) shootStart0 = 1 ;   
            if( strcmp("S",buffer) == 0) direction = 0 ;               
            //sprintf(buffer,"%d",argb) ;
            //MessageBox(*myHwnd, buffer , "UDP receiver", MB_OK);
            //send sth to the client
            //sendto(socket1,buffer,sizeof(buffer),0,(struct sockaddr*)&client,len);
        }                                              
     }
     closesocket(socket1);
     
     return ;
}
char* getLocapIpAddress(){
    char ac[80];
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        return NULL;
    }
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)return NULL;    
    struct hostent *phe = gethostbyname(ac);
    if (phe == 0)return NULL;    
    
    struct in_addr addr;
    memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
    WSACleanup();
    return inet_ntoa(addr);
}
