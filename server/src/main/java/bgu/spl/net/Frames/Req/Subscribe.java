package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Subscribe extends RequestFrame {
    private String channelName;
    private int channelId;


    public Subscribe(String msg) {
        super(-1,msg);
        String[] lines = msg.split("\n");
        int recipt=-1;
        for(String line : lines){
            int colonIndex = msg.indexOf(":");
            String key = msg.substring(0, colonIndex).trim();
            // Extract the value (after the colon)
            String value = msg.substring(colonIndex + 1);
            if(key=="recipt")
                recipt = Integer.parseInt(value);
            else if(key=="destination")
                channelName = value;
            else if(key=="id")
                channelId =  Integer.parseInt(value);
        }
        this.setRecipet(recipt);

    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f;
        if(!c.isLoggedIn(id))
            f=new bgu.spl.net.Frames.Res.Error("user not loged in so can not subscribe to channels", this.getMessage(), "user not loged in so can not subscribe to channels", id);
        else if(c.isSubscribe(id, channelName))
            f=new bgu.spl.net.Frames.Res.Error("user already subscribe to this channel", this.getMessage(), "user already subscribe to this channel", id);
        else{
            c.addSubscribtion(channelName,id,channelId);
            f=new Reciept(id);
        }
        c.send(id, f);
    }
}
