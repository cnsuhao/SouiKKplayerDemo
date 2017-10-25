#include "VideoBitmap.h"
namespace SOUI{
	   SVideoBitmap::SVideoBitmap():m_hBmp(0)
        {
			m_sz.cx=m_sz.cy=0;
			m_pBmpBits=0;
			m_bufLen=0;
        }
        SVideoBitmap::~SVideoBitmap()
        {
            if(m_hBmp) 
				DeleteObject(m_hBmp);
        }
        HRESULT SVideoBitmap::Init(int nWid,int nHei,const LPVOID pBits)
		{
		    if(m_hBmp) 
			{
				
			  DeleteObject(m_hBmp);
			  m_pBmpBits=0;
			  m_bufLen=0;
			}
			
			m_hBmp = CreateGDIBitmap(nWid,nHei,&m_pBmpBits);
			if(m_hBmp)
			{
				m_sz.cx=nWid,m_sz.cy=nHei;
				const int stride = m_sz.cx*4;
				m_bufLen=stride*m_sz.cy;
				if(pBits)
				{
					 memcpy(m_pBmpBits,pBits,m_bufLen);
				}else
				{
					 memset(m_pBmpBits,0,m_bufLen);
				}
			}
			return m_hBmp?S_OK:E_OUTOFMEMORY;
		}
        HRESULT SVideoBitmap::Init(IImgFrame *pFrame)
		{
			    UINT nWid,nHei;
			    pFrame->GetSize(&nWid,&nHei);

				if(m_hBmp) DeleteObject(m_hBmp);
				void * pBits=NULL;
				m_hBmp = CreateGDIBitmap(nWid,nHei,&pBits);
				if(!m_hBmp) return E_OUTOFMEMORY;

				m_sz.cx=nWid,m_sz.cy=nHei;
				const int stride = m_sz.cx*4;
				pFrame->CopyPixels(NULL, stride, stride * m_sz.cy,
					reinterpret_cast<BYTE*>(pBits));
				return S_OK;
		}
        HRESULT SVideoBitmap::LoadFromFile(LPCTSTR pszFileName)
		{
		     return S_FALSE;
		}
        HRESULT SVideoBitmap::LoadFromMemory(LPBYTE pBuf,size_t szLen)
		{
		     return S_FALSE;
		}

        UINT SVideoBitmap::Width() const
		{
		     return m_sz.cx;
		}
        UINT SVideoBitmap::Height() const
		{
		      return m_sz.cy;
		}
        SIZE SVideoBitmap::Size() const
		{
		     return m_sz;
		}
        LPVOID  SVideoBitmap::LockPixelBits()
		{
		    BITMAP bm;
			GetObject(m_hBmp,sizeof(bm),&bm);
			return bm.bmBits;
		}
        void    SVideoBitmap::UnlockPixelBits(LPVOID)
		{
		
		}
        const LPVOID SVideoBitmap::GetPixelBits() const
		{
		    BITMAP bm;
			GetObject(m_hBmp,sizeof(bm),&bm);
			return bm.bmBits;
		}
		IRenderFactory * SVideoBitmap::GetRenderFactory() const
		{
		    return NULL;
		}
		 //////////////////////////////////////////////////////////////////////////
		//  SBitmap_GDI
		HBITMAP SVideoBitmap::CreateGDIBitmap( int nWid,int nHei,void ** ppBits )
		{
			BITMAPINFO bmi;
			memset(&bmi, 0, sizeof(bmi));
			bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth       = nWid;
			bmi.bmiHeader.biHeight      = -nHei; // top-down image 
			bmi.bmiHeader.biPlanes      = 1;
			bmi.bmiHeader.biBitCount    = 32;
			bmi.bmiHeader.biCompression = BI_RGB;
			bmi.bmiHeader.biSizeImage   = 0;

			HDC hdc=GetDC(NULL);
			HBITMAP hBmp=CreateDIBSection(hdc,&bmi,DIB_RGB_COLORS,ppBits,0,0);
			ReleaseDC(NULL,hdc);
			return hBmp;
		}
        
}