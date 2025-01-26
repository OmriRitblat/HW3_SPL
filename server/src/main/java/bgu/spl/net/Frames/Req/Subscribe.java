package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

import java.util.concurrent.ConcurrentHashMap;

public class Subscribe extends RequestFrame {
    private String channelName;
    private int channelId;


    public Subscribe(String msg) {
        super(-1,msg,"Subscribe");
        String[] lines = msg.split("\n");
        boolean isIdFound=false;
        int recipt=-1;
        ConcurrentHashMap<String,String> data = super.getHeaders();
        if(data.containsKey("receipt"))
            recipt = Integer.parseInt(data.get("receipt"));
        if(data.containsKey("id")) {
            channelId = Integer.parseInt(data.get("id"));
            isIdFound = true;
        }

        if(data.containsKey("destination"))
            channelName = data.get("destination");
        this.setRecipet(recipt);
        if(channelName==null || !isIdFound)
            this.setMissingData(true);
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f=null;
        if(this.isMissingData())
            f=new Error("part of the data is missing, please send {destination, id} in order to subscribe", this.getMessage(),"the frame missing data",this.getReciept());
        else if(!c.isLoggedIn(id))
            f=new bgu.spl.net.Frames.Res.Error("user not loged in so can not subscribe to channels", this.getMessage(), "user not loged in so can not subscribe to channels", id);
        else if(c.isSubscribe(id, channelName))
            f=new bgu.spl.net.Frames.Res.Error("user already subscribe to this channel", this.getMessage(), "user already subscribe to this channel", id);
            else if(headersCheck(2))
        f = new Error("there are to much headers", this.getMessage(),
        "headers check failed", this.getReciept());
        else{
            c.addSubscribtion(channelName,id,channelId);
            if(this.getReciept()!=-1)
                f=new Reciept(this.getReciept());
        }
        if(f!=null)
            c.send(id, f.toString());
        if(f!=null && f.getClass().equals(Error.class))
            c.disconnect(id);
    }
}
