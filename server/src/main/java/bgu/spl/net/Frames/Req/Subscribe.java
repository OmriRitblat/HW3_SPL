package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Subscribe extends RequestFrame {
    private String channelName;


    public Subscribe(String msg) {
        super(-1,msg);
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f;
        if(!c.isLoggedIn(id))
            f=new bgu.spl.net.Frames.Res.Error("user not loged in so can not subscribe to channels", this.getMessage(), "user not loged in so can not subscribe to channels", id);
        else if(c.isSubscribe(id, channelName))
            f=new bgu.spl.net.Frames.Res.Error("user already subscribe to this channel", this.getMessage(), "user already subscribe to this channel", id);
        else{
            c.addSubscribtion(channelName,id);
            f=new Reciept(id);
        }
        c.send(id, f);
    }
}
