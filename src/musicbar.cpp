#include <cstring>
#include <iostream>

#include "musicbar.h"
#include "status.h"

#include <wx/wx.h>
#include <wx/dcbuffer.h>

using namespace std;

MusicBar::MusicBar(wxWindow* parent, wxPoint barpos, wxSize barsize)
    : wxPanel(parent, ID_MUSICBAR,
              barpos, barsize, wxBORDER_NONE, "MusicBar")
{
    none.Set("#444444");
    gray.Set("#A8A8A8");
    blue.Set("#0808FF");
    yellow.Set("#FFFF08");
    border.Set("#000000");
    position.Set("#FF0000");
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    side_ = barsize.GetHeight() / 12;

    Bind(wxEVT_PAINT, &MusicBar::OnPaint, this);

    size_ = barsize;
    pos_ = barpos;
    length_ = 0;
    now_ = 0;
    judge_ = new int[120];

    for (int i = 0; i < 120; i++)
        judge_[i] = 0;
    

}

void MusicBar::Set(Song* song)
{
    bar_ = song->GetMusicBar();
    length_ = song->GetLength() + 3000;
    Clean();
    return;
}


void MusicBar::Result(int index, int judge)
{
    int result;

    switch (judge) {
        case 4:
            result = 3;
            break;
        case 3:
            result = 2;
            break;
        case 2:
            result = 2;
            break;
        case 1:
            result = 1;
            break;
        case 0:
            result = 1;
            break;
    }

    if (judge_[index] > result)
        judge_[index] = result;

    RefreshRect(wxRect(side_ * index, 0, side_, side_ * 12));
    return;
}

void MusicBar::Now(int n)
{
    bool out = true;
    now_ = n;

    if (now_ > length_) {
        now_ = length_;
        out = false;
    }

    return;
}

int MusicBar::GetNow()
{
    return now_;
}

bool MusicBar::NowRefresh(int n)
{
    bool out = true;
    int j = 120 * n / length_;
    int j1 = 120 * now_ / length_;

    if (j1 != j)
        judge_[j] = 4;

    int x = side_ * 120 * n / length_;
    x -= side_;
    int x1 = side_ * 120 * now_ / length_;
    x1 -= side_;

    if (x1 != x) {
        now_ = n;

        if (now_ > length_) {
            now_ = length_;
            out = false;
        }
        else {
            RefreshRect(wxRect(x, 0, side_ * 2, side_ * 12));
        }
    }

    return out;
}

void MusicBar::Clean()
{
    now_ = 0;

    for (int i = 0; i < 120; i++)
        judge_[i] = 0;

    Refresh();
}

int MusicBar::jump(int pixelPosition)
{
    now_ = pixelPosition * length_ / size_.GetWidth();
    return now_;
}

void MusicBar::loadRecord()
{
    return;
}

void MusicBar::show(wxDC& dc)
{
    for (int i = 0; i < 120; i++) {
        switch (judge_[i]) {
            case 0:
                dc.SetPen(wxPen(gray));
                dc.SetBrush(wxBrush(gray));
                break;
            case 1:
            case 4:
                dc.SetPen(wxPen(none));
                dc.SetBrush(wxBrush(none));
                break;
            case 2:
                dc.SetPen(wxPen(blue));
                dc.SetBrush(wxBrush(blue));
                break;
            case 3:
                dc.SetPen(wxPen(yellow));
                dc.SetBrush(wxBrush(yellow));
                break;
        }

        int n = bar_[i];

        if (n > 8)
            n = 8;

        dc.DrawRectangle(side_ * i, side_ * (12 - n) - 1,
                         side_, side_ * n);
        dc.SetPen(wxPen(border));
        dc.DrawLine(side_ * i - 1, 0,
                    side_ * i - 1, side_ * 12);
    }

    dc.SetBrush(wxBrush(border));

    for (int i = 0; i < 12; i++) {
        dc.DrawLine(0, side_ * (11 - i) - 1,
                    side_ * 120, side_ * (11 - i) - 1);
    }

    //dc.SetPen(wxPen(position));
    //dc.DrawLine(side_ * 120 * now_ / length_, 0,
                //side_ * 120 * now_ / length_, side_ * 12);
    
}

void MusicBar::OnPaint(wxPaintEvent& evt)
{
    wxBufferedPaintDC dc(this);
    dc.SetBackground(*wxTRANSPARENT_BRUSH);
    dc.Clear();

    switch (status) {
        case S_WEL:
            break;
        case S_CH:
            loadRecord();
            show(dc);
            break;
        case S_PLAY:
            show(dc);
            dc.SetPen(wxPen(position));
            dc.DrawLine(side_ * 120 * now_ / length_, 0,
                        side_ * 120 * now_ / length_, side_ * 12);
            break;
        case S_OPT:
            break;
    }

    return;
}
