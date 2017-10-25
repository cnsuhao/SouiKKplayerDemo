#include "ui.h"
#include "AVPlayer/IAVPlayer.h"
#include <string>
#include "VideoBitmap.h"

#ifndef SUIVIDEO_H_
#define SUIVIDEO_H_
/********视频播放器控件**************/
namespace SOUI
{
      class  SVideoPlayer: public SWindow, public ITimelineHandler,public IAVPlayerNotify
	  {
		     SOUI_CLASS_NAME( SVideoPlayer, L"videoplayer")
	  public:
			  SVideoPlayer(void);
			  ~SVideoPlayer(void);
			  void SetAVVisible(BOOL bVisible);
			  ///设置显示名称
			  void SetDisplayName(std::wstring DisName);
              void GetDisplayName(std::wstring &DisName);
			  int  OpenMedia(const char *str,int Vol);
			  void Close(bool hide=true);
			  void SetPlayStat(int state);
			  void SetVolume(long value);
			  void SetVolume(long value,bool tip);
			  void SetAVPlayerNotify(IAVPlayerNotify* nty);
			  void CreatePlayer(HWND fh,DWORD style=WS_CHILDWINDOW | WS_CLIPCHILDREN| WS_CLIPSIBLINGS,bool yuv420=true);
			  HWND GetPlayerHwnd();
			  void SetUserId(int UserId);
			  bool OnSliderAudioVol(EventArgs *pEvt);
			  void GetUrl(std::string &Url){Url=m_Strurl;};
	  protected:
		      void OnPlayerState(void* UserData,AVPALYERSTATE state);
		      static void RenderImgCall (kkAVPicRAWInfo* data,void* UserData);
			  void OnNextFrame();
			  void RenderImg(kkAVPicRAWInfo* data);
			  void OnDestroy();
			  int  OnCreate(void*);
			  void OnPaint(IRenderTarget *pRT);
			  void OnSize(UINT nType, CSize size);
			  void OnMouseHover(WPARAM wParam, CPoint ptPos);
			  void OnMouseLeave();
              BOOL OnEraseBkgnd(IRenderTarget *pRT);
			  
			SOUI_MSG_MAP_BEGIN()
					MSG_WM_ERASEBKGND_EX(OnEraseBkgnd)
					MSG_WM_CREATE(OnCreate)
					MSG_WM_DESTROY(OnDestroy)
					MSG_WM_PAINT_EX(OnPaint)
					MSG_WM_SIZE(OnSize)
					MSG_WM_MOUSEHOVER(OnMouseHover)
				
			SOUI_MSG_MAP_END()


		   HRESULT OnAttrDui(const SStringW& strValue, BOOL bLoading);
		   SOUI_ATTRS_BEGIN()
                  ATTR_CUSTOM(L"dui", OnAttrDui)
		   SOUI_ATTRS_END()
	  private:
		    SCriticalSection    m_lock;
		    std::string         m_Strurl;
			std::wstring        m_StrDisName;
			std::wstring        m_StrDuiErr;
			int                 m_UserId;
		    int                 m_nDui;
			
			SVideoBitmap        m_VideoBitmap;
			HDC                 m_memdc;
            int                 m_Tick;
			IAVPlayerNotify*    m_pAVPlayerNty;

		    //播放器接口
		    IAVPlayer*          m_pPlayer;
			//播放器窗口句柄
			HWND                m_hPlayerWnd;
			//父窗口句柄
			HWND                m_hParenthWnd;
			int                 m_nDcBltMode;
			int                 m_nDestWnd;
	  };
}
#endif