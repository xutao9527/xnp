#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include "XnpFrame.h"

// IMPLEMENT_DYNAMIC_CLASS(XnpFrame, wxFrame)

wxBEGIN_EVENT_TABLE(XnpFrame, wxFrame)
                EVT_PAINT(XnpFrame::OnPaint)
wxEND_EVENT_TABLE()

void XnpFrame::OnPaint(wxPaintEvent& e){

    //wxBufferedPaintDC dc(this);
    //if(image.IsOk()){
    //    dc.DrawBitmap(image.Scale(image.GetWidth(),image.GetHeight()),0,0,true);
    //}

}