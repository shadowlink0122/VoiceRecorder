
#pragma once

struct mouseParam{
    int x;
    int y;
    int event;
    int flags;
};

void CallBackFunc(int eventType, int x, int y, int flags, void* userdata){
    mouseParam *ptr = static_cast<mouseParam*> (userdata);

    ptr->x = x;
    ptr->y = y;
    ptr->event = eventType;
    ptr->flags = flags;
}

bool click_record(mouseParam &mouse,int w,int h){
    
    if((w*0.1-20 <= mouse.x && mouse.x <= w*0.5-20) && (h*0.1-20 <= mouse.y && mouse.y <= h*0.25-20))
        return true;

    return false;
}

bool click_stop(mouseParam &mouse,int w,int h){
    
    if((w*0.5+20 <= mouse.x && mouse.x <= w*0.9+20) && (h*0.1-20 <= mouse.y && mouse.y <= h*0.25-20))
        return true;

    return false;
}

bool click_play(mouseParam &mouse,int w,int h){
    
    if((w*0.1-20 <= mouse.x && mouse.x <= w*0.5-20) && (h*0.3-20 <= mouse.y && mouse.y <= h*0.45-20))
        return true;
    
    return false;
}

bool click_exit(mouseParam &mouse,int w,int h){

    if((w*0.5+20 <= mouse.x && mouse.x <= w*0.9+20) && (h*0.3-20 <= mouse.y && mouse.y <= h*0.45-20))
        return true;

    return false;
}
