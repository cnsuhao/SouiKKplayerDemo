#include "ui.h"
#include "SVideoPlayer.h"
#include <string>
#include "Player/KKPlayer.h"


namespace SOUI
{
	SVideoPlayer::SVideoPlayer(void):m_pPlayer(0),m_hPlayerWnd(0),m_pAVPlayerNty(0),m_hParenthWnd(0),m_nDui(0)
		,m_memdc(0),m_Tick(0),m_UserId(0),m_nDcBltMode(1), m_nDestWnd(0)
	{

	}
	void SVideoPlayer::SetUserId(int UserId)
	{
	   m_UserId=UserId;
	}
	SVideoPlayer::~SVideoPlayer(void)
	{
		if(m_pPlayer!=NULL){
		    delete m_pPlayer;
		}
		m_pPlayer=0;
	}
	void SVideoPlayer::SetAVPlayerNotify(IAVPlayerNotify* nty)
	{
	   m_pAVPlayerNty=nty;
	}
	void SVideoPlayer::SetAVVisible(BOOL bVisible)
	{
		if(::IsWindow(m_hPlayerWnd))
		{
			if(bVisible==TRUE){
				::ShowWindow(m_hPlayerWnd,SW_SHOW);
			}else{
				::ShowWindow(m_hPlayerWnd,SW_HIDE);
			}
		}
	}
	
	void SVideoPlayer::SetPlayStat(int state)
	{
       
	}
	void SVideoPlayer::OnDestroy()
	{
		this->m_lock.Enter();
		 m_nDestWnd=1;
		 this->m_lock.Leave();
          Close();
		  if(m_nDui){
			  GetContainer()->UnregisterTimelineHandler(this);
		  }else{
	         ::DestroyWindow(m_hPlayerWnd);
		  }
		  if(!m_style.m_bBkgndBlend)
		{
			if(m_invalidRegion)
			{
			    m_invalidRegion=NULL;
			}
		
		}
	}
	void SVideoPlayer::CreatePlayer(HWND fh,DWORD style,bool yuv420)
	{
	    CKKPlayer *pKK = new CKKPlayer();
		RECT rt={0,100,200,300};
		m_hParenthWnd=fh;
		m_hPlayerWnd=pKK->CreateKKPlayer(fh,rt, style,yuv420);
		pKK->SetUserData(this);
		m_pPlayer=pKK;
	}
	HWND SVideoPlayer::GetPlayerHwnd()
	{
	   return m_hPlayerWnd;
	}
	int  SVideoPlayer::OnCreate(void* p)
	{
		int ll= __super::OnCreate(NULL);
		HWND h=GetContainer()->GetHostHwnd();
		
		if(m_nDui){
			  m_memdc=CreateCompatibleDC(0);
		      CKKPlayer *pKK = new CKKPlayer();
			  pKK->CreateDuiKKPlayer(h,SVideoPlayer::RenderImgCall,this);
		      pKK->SetUserData(this);
		      m_pPlayer=pKK;
			  GetContainer()->RegisterTimelineHandler(this);
			
		}else{
			DWORD style= WS_CHILDWINDOW |WS_CLIPCHILDREN| WS_CLIPSIBLINGS;
		    CreatePlayer(h,style,true);
		    ::SetWindowPos(h,HWND_BOTTOM ,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
        return ll;
	}
	bool SVideoPlayer::OnSliderAudioVol(EventArgs *pEvt)
	{
	    EventSliderPos *pEvt2 = sobj_cast<EventSliderPos>(pEvt);
		SASSERT(pEvt2);
		SSliderBar * pSlider = sobj_cast<SSliderBar>(pEvt->sender);
	    long vol=pEvt2->nPos;
		SetVolume(vol,true);

		SStringT tip;
		tip.Format(L"ÒôÁ¿£º%d%%",vol);
		pSlider->SetAttribute(L"tip",tip);
		return true;
	}
	void SVideoPlayer::OnNextFrame()
	{
		 //this->Invalidate();
	         int Tick=::GetTickCount();
			 //Ë¢ÐÂ10Ö¡/s¡£//
			 if(Tick-m_Tick>100){
				SWindow* pWin=this->FindChildByName("VideoVolSliderBar");
				if( pWin!=NULL&&pWin->IsVisible()){
					CRect rt=this->GetWindowRect();
					POINT pt;
					GetCursorPos(&pt);
					::ScreenToClient(this->GetContainer()->GetHostHwnd(),&pt);
					if(!rt.PtInRect(pt))
					{
		     			pWin->SetVisible(0);
						pWin=this->FindChildByName("WinPlayerCut");
						pWin->SetVisible(0);
					}
				}

				 m_Tick=Tick;
				 if(m_pPlayer!=NULL){
					 m_pPlayer->RefreshDuiPlayer();
				 }
			     this->Invalidate();
			 }
	}
	void SVideoPlayer::OnPlayerState(void* UserData,AVPALYERSTATE state)
	{
             if(!m_nDui)
				 return;
			 if(state==AVOpenFailed){
				    m_StrDuiErr=L"Á´½ÓÊ§°Ü";
			 }
	}
	void SVideoPlayer::RenderImgCall (kkAVPicRAWInfo* data,void* UserData)
	{
	    SVideoPlayer *ply=(SVideoPlayer *) UserData;
		if(ply!=NULL&&data!=NULL){
		   ply->RenderImg(data);
		}
	}
	void SVideoPlayer::RenderImg(kkAVPicRAWInfo* data)
	{
		m_lock.Enter();
		if( m_nDestWnd==1){
			m_lock.Leave();
		   return ;
		}
		if(data->width==m_VideoBitmap.Width()&& data->height==m_VideoBitmap.Height()){
			 LPVOID  bufd=m_VideoBitmap.LockPixelBits();
			 memcpy(bufd,data->data[0],data->linesize[0]*data->height);
			 m_VideoBitmap.UnlockPixelBits(bufd);  
			
		  }else{
			  m_VideoBitmap.Init(data->width,data->height);
			  HBITMAP bmp=m_VideoBitmap.GetBitmap();
			  ::SelectObject(m_memdc,bmp);
		}
		m_lock.Leave();
		//
	}
	void SVideoPlayer::SetVolume(long value)
	{
		bool tip=true;
		if(m_nDui)
			tip=false;

		if(m_pPlayer!=NULL)
           m_pPlayer->SetVlcVolume(value,tip);
	}
	void SVideoPlayer::SetVolume(long value,bool tip)
	{
	    if(m_pPlayer!=NULL)
           m_pPlayer->SetVlcVolume(value,tip);
	}
	BOOL SVideoPlayer::OnEraseBkgnd(IRenderTarget *pRT)
	{
		if(m_nDui){
	        return 1;
		}else{
         return  __super::OnEraseBkgnd(pRT);
		}
	}
	void SVideoPlayer::OnPaint(IRenderTarget *pRT)
	{
		if(m_nDui){
		   CRect rcClient;
		   GetClientRect(&rcClient);
		   int ww=rcClient.right-rcClient.left;
		   int wh=rcClient.bottom-rcClient.top;
		   int w=0,h=0; 
		   HDC m_hdc =pRT->GetDC(); 
		   if(m_nDcBltMode){
			   m_nDcBltMode=0;
			   SetStretchBltMode(m_hdc,HALFTONE);
			   
		   }
		   m_lock.Enter();
		   w=m_VideoBitmap.Width();
		   h=m_VideoBitmap.Height();
           
		   if(ww!=w&&h!=wh){
		      ((CKKPlayer *)m_pPlayer)->DuiSize(ww,wh);
		   }
		  
		  
		   //rcClient.left, rcClient.top
		   StretchBlt( m_hdc, rcClient.left, rcClient.top,ww ,wh, m_memdc, 0, 0,w,h, SRCCOPY);
		   m_lock.Leave();
          
		  
		}else{
          __super::OnPaint(pRT);
		}
	}
	void  SVideoPlayer::OnSize(UINT nType, CSize size)
	{
        __super::OnSize(nType,size);
		if(m_hParenthWnd!=0&&::IsWindow(m_hPlayerWnd))
		{
			RECT rt;
			this->GetWindowRect(&rt);

			int  cx   =   GetSystemMetrics(SM_CXSCREEN);   
			int  cy   =   GetSystemMetrics(SM_CYSCREEN);
			RECT rtx;
			::GetWindowRect(::GetParent(m_hPlayerWnd),&rtx);
			int h=rtx.bottom-rtx.top;
			int w=rtx.right-rtx.left;
			if(w>=cx&&h>=cy){
                   ::SetWindowPos(m_hPlayerWnd,0,0,0,cx,cy,SWP_NOZORDER);
			}else{
                   ::SetWindowPos(m_hPlayerWnd,0,rt.left,rt.top,size.cx,size.cy,SWP_NOZORDER);
			}
		}else if(m_nDui){
            RECT rt;
			this->GetWindowRect(&rt);
			int h=0,w=0;
			h=rt.bottom-rt.top;
			w=rt.right-rt.left;
			((CKKPlayer *)m_pPlayer)->DuiSize(w,h);
		}
	}
	
	void SVideoPlayer::OnMouseHover(WPARAM wParam, CPoint ptPos)
	{
		
	}
	
	void SVideoPlayer::Close(bool hide)
	{
		  if(m_pPlayer!=NULL){
			m_pPlayer->CloseMedia();
			if(m_hParenthWnd!=0&&hide)
			::ShowWindow(m_hPlayerWnd,SW_HIDE);
		 }
	}

	int is_realtime2(char *str)
	{
		if(   !strcmp(str, "rtp")    || 
			!strcmp(str, "rtsp")   || 
			!strcmp(str, "sdp")
			)
			return 1;


		if( !strncmp(str, "rtmp:",5))
		{
			return 1;
		}
		if(   !strncmp(str, "rtp:", 4)|| 
			!strncmp(str, "udp:", 4) 
			)
			return 1;
		return 0;
	}
	
	void SVideoPlayer::SetDisplayName(std::wstring DisName)
	{
		this->m_StrDisName=DisName;
	}
	void SVideoPlayer::GetDisplayName(std::wstring &DisName)
	{
	    DisName=this->m_StrDisName;
	}
	int SVideoPlayer::OpenMedia(const char *str,int Vol)
	 {
		/* static int xx=0;
		 if(xx)
			 return 1;
		 str="D:\\testvideo\\ff.mp4";*/
		 m_Strurl=str;
		 int ret=0;
		// ::ShowWindow(m_hPlayerWnd,SW_SHOW);
		 if(m_pPlayer!=NULL){
			 bool tip=true;
		     if(m_nDui){
			     tip=false;
				 m_pPlayer->SetAVPlayerNotify(this);
			 }
			 else
		         m_pPlayer->SetAVPlayerNotify(m_pAVPlayerNty);

			m_pPlayer->SetVlcVolume(Vol,tip);
			m_pPlayer->CloseMedia();
			ret=m_pPlayer->OpenMedia(str,"-fflags nobuffer");
			m_pPlayer->SetMaxRealtimeDelay(1);
		 }
         return ret;
	 }
	 HRESULT SVideoPlayer::OnAttrDui(const SStringW& strValue, BOOL bLoading)
	 {
	       m_nDui= _wtoi(strValue);
		   return 1;
	 }
}