package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Unsubscribe extends RequestFrame {
    private int channelId;


    public Unsubscribe(String msg) {
        super(-1,msg);
        String[] lines = msg.split("\n");
        int recipt=-1;
        boolean isIdFound=false;
        for(String line : lines){
            int colonIndex = msg.indexOf(":");
            String key = msg.substring(0, colonIndex).trim();
            // Extract the value (after the colon)
            String value = msg.substring(colonIndex + 1);
            if(key=="recipt")
                recipt = Integer.parseInt(value);
            else if(key=="id") {
                isIdFound = true;
                channelId = Integer.parseInt(value);
            }
        }
        this.setRecipet(recipt);
        if(!isIdFound){
            this.setMissingData(true);
        }
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f;
        if(this.isMissingData())
            f=new Error("part of the data is missing, please send {id} in order to connect", this.getMessage(),"the frame missing data",this.getReciept());
        else if(!c.isLoggedIn(id))
            f=new bgu.spl.net.Frames.Res.Error("user not loged in so can not unsubscribe to channels", this.getMessage(), "user not loged in so can not unsubscribe to channels", id);
        else if(!c.isSubscribe(id, channelId))
            f=new bgu.spl.net.Frames.Res.Error("user is not subscribe to this channel or channel does not exist so can not unsubscribe", this.getMessage(), "user is not subscribe to this channel so can not unsubscribe", id);
        else{
            c.removeSubscribtion(channelId,id);
            f=new Reciept(id);
        }
        c.send(id, f);
    }
}
