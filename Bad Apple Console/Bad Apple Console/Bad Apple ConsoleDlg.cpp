
// Bad Apple ConsoleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Bad Apple Console.h"
#include "Bad Apple ConsoleDlg.h"
#include "afxdialogex.h"
#include "MyTimer.h"
#include "mmsystem.h"
#include <afxwin.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

using namespace std;

void play();
static int time_already_play;
static MCI_OPEN_PARMS mciOpen;
#define NUM 5450;           //ͼƬ������
int NUM_x = NUM;
clock_t startclk,endclk;

//5ͼ��Ӧx��1��
int* sync = new int[NUM_x/5];            //����ʵ�ʽ����е�ÿ����

void NewSleep(int);   
int background = 0;

MyTimer timer;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBadAppleConsoleDlg �Ի���




CBadAppleConsoleDlg::CBadAppleConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBadAppleConsoleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CBadAppleConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBadAppleConsoleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBadAppleConsoleDlg ��Ϣ�������

BOOL CBadAppleConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//���ڷ������+��С��
	WINDOWPLACEMENT lwndpl;
	WINDOWPLACEMENT * lpwndpl;
	lpwndpl=&lwndpl;
	GetWindowPlacement(lpwndpl);
	lpwndpl->showCmd=SW_SHOWMINIMIZED;
	SetWindowPlacement(lpwndpl);
	this->ModifyStyle(0,WS_OVERLAPPEDWINDOW);
	this->SetWindowText("BadApple Console v1.0");

	//console ����
	::AllocConsole();    // �򿪿ؼ�̨��Դ
    freopen("CONOUT$", "w+t", stdout);    // ����д	
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // ��ȡ��׼����豸���
	SetConsoleTitle("Bad Apple CMD v1.0");       //����console����
	SMALL_RECT rc = {0,0, 80-1, 30}; // ���ô���λ�úʹ�С�����򳬹�79�Ļ����þͻ�ʧ��
	SetConsoleWindowInfo(hOut,true ,&rc);

	PlayGameSound("Bad Apple.mp3");
	play();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CBadAppleConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBadAppleConsoleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBadAppleConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void play(){
	//������ԴIDR_ZIP1
	HINSTANCE hInstance=GetModuleHandle(0);
	HRSRC hrsrc = FindResource(NULL,MAKEINTRESOURCE(IDR_ZIP2),"ZIP");  //�ڶ���������������ԴID��������������
	HANDLE hglob = LoadResource(hInstance,hrsrc);	//������Դ
	void *zipbuf = LockResource(hglob);	//�õ�ָ����Դ��ָ��
	unsigned int ziplen=SizeofResource(hInstance,hrsrc);//��ȡ��Դ�ĳߴ�
	
	//zip��ز���PART_1
	HZIP hz;	
	ZIPENTRY ze;
	hz = OpenZip(zipbuf, ziplen, 0);  
  	GetZipItem(hz,-1,&ze);  //�ڶ���������-1�����ȡzip�е������ļ�
	int numitems = ze.index;	//��ȡzip�а������ļ��ĸ�����Ϊ��һ��ѭ����ȡͼƬ��׼��

	CImage img;
	string frame_output=""; //һ֡���
	//--------------------------------�ü������Դ�ļ�Bad Apple pics.zip���--------------------------
	int count = 0;
	for (int zi=0; zi<numitems; zi++)
	{ 
		//zip��ز���PART_2
		ZIPENTRY ze; 
		GetZipItem(hz,zi,&ze); //fetch individual details,�ڶ�����������Ŵ����ȡzip�еĵڼ����ļ�
		string a("C:\\temp\\");
		string k = ze.name;
		a += k;
		UnzipItem(hz, zi, a.c_str());  //��ѹͼƬ��·��C:\temp\XXX.jpg
		count++;
		img.Load(a.c_str());
		DeleteFile(a.c_str());//Ѹ��ɾ��ͼƬʹ�����ۼ�
		int i, j; 
		//ͼ��ÿһ�е��ֽ���
		int nRowBytes = img.GetPitch(); 						
		//���nRowBytes>0  ��ӿ�ʼ����β
		//���nRowBytes<0, ��ӽ�β����ʼ
		int nWidth = img.GetWidth(); 
		int nHeight = img.GetHeight(); 
		//ÿ��������ռ���ֽ���
		int nClrCount = img.GetBPP() / 8;     //BPP=bits per pixel
		BYTE* p; 
		p = (BYTE*)img.GetBits(); 
		//if (nRowBytes<0)                             //ʹpָ���һ�е���ʼ����
		//	p += nRowBytes*(nHeight-1);
		for(i = 0; i < nHeight; i++) { 			
			for(j = 0; j < nWidth; j++) { 
				//�洢��ʽRGBRGBRGB...
				int average;       //ƽ�����Ͷ�
				average = ( p[j*nClrCount]+p[j*nClrCount+1]+p[j*nClrCount+2] )/3;
				//*(start+average) = *(start+average) + 1;     //�������һ��+1
				if (average>200){
					frame_output += " ";
				}
				else
					frame_output+="#";
			} 
			if (i<nHeight-1)              //���һ�оͲ����ٻ���
				p += nRowBytes;        //����
		}  			
		img.Destroy();	//Destroy����һ�β��ܶ�ȡ
		if (count == 1){
			while(1){
				if(time_already_play>200)
					break;
			}
			startclk = clock();                     //��ʱ��ʼ
		}
		printf("%s",frame_output.c_str());     //���������һ֡		
		frame_output = "";      //�������ַ���

		//�ƶ����������
		COORD coord;
		coord.X=0;
		coord.Y=10;		
		HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hout,coord);
		
		//ʵ�־�ȷ��ʱ������Ŀǰ���Ⱥͱ�׼���ȵ�ʱ��������ʱ���̣�������sleep(1)Ϊ��λ��С���
		endclk = clock();
		float std_time = 218000*float(count)/NUM;
		while (endclk - startclk < std_time){
			timer.Sleep(1);
			endclk = clock();
		}	
	}
	CloseZip(hz);
}

CBadAppleConsoleDlg::~CBadAppleConsoleDlg(){
	FreeConsole();                      // �ͷſ���̨��Դ
}

UINT CBadAppleConsoleDlg::ThreadPlaySound(LPVOID pFileName)
{
    //srand((DWORD)time(NULL));
    //MCIDEVICEID  dev[20]={0};

    MCI_PLAY_PARMS mciPlay;
    try
    {    
        mciOpen.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
        mciOpen.lpstrElementName = (LPCSTR)pFileName;
        mciOpen.wDeviceID = 1;
        if(mciSendCommand(NULL,MCI_OPEN, MCI_OPEN_ELEMENT,(DWORD_PTR)&mciOpen)!=0)
        {    
            return 0 ;
        }
        mciPlay.dwFrom = 0;
        mciPlay.dwTo = 100;
        mciSendCommand(1,MCI_PLAY,MCI_FROM|MCI_NOTIFY,(DWORD_PTR)(LPVOID)&mciPlay);
		while(1){
			MCI_STATUS_PARMS StatusParms; 
			StatusParms.dwItem = MCI_STATUS_POSITION;
			mciSendCommand (1, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&StatusParms);
			time_already_play = int(StatusParms.dwReturn);
		}
    }
    catch (...)
    {

    }
    if(pFileName!=NULL)
        delete pFileName;
    return 0 ; 

}

//��������,static
void CBadAppleConsoleDlg::PlayGameSound(LPCTSTR pszSoundName)
{        
    //������ָ̬��,�ǵ�Ҫɾ��
    size_t size = (strlen(pszSoundName)+1)*sizeof(char);
    char* pChar = (char*)malloc(size);
    memset(pChar,0,size);
    strcpy_s(pChar,size,pszSoundName);
    CWinThread* pTh = AfxBeginThread(ThreadPlaySound,(LPVOID)pChar,0,0,NULL);
}

