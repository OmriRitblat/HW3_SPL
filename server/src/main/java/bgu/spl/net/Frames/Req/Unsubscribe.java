package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Unsubscribe extends RequestFrame {
    private int channelId;


    public Unsubscribe(String msg) {
        super(-1,msg,"Unsubscribe");
        String[] lines = msg.split("\n");
        int recipt=-1;
        boolean isIdFound=false;
        boolean isreciptExist=false;
        if(!super.getHeaderByKey("id").equals("")) {
            isIdFound = true;
            channelId = Integer.parseInt(super.getHeaderByKey("id"));
        }
        if(!super.getHeaderByKey("receipt").equals("")) {
            isreciptExist = true;
            recipt=(Integer.parseInt(super.getHeaderByKey("receipt")));
        }
        if(isreciptExist)
            this.setRecipet(recipt);
        if(!isIdFound){
            this.setMissingData(true);
        }
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f=null;
        if(this.isMissingData())
            f=new Error("part of the data is missing, please send {id} in order to UNSUBSCRIBE", this.getMessage(),"the frame missing data",this.getReciept());
        else if(!c.isLoggedIn(id))
            f=new bgu.spl.net.Frames.Res.Error("user not loged in so can not unsubscribe to channels", this.getMessage(), "user not loged in so can not unsubscribe to channels", id);
        else if(!c.isSubscribe(id, channelId))
            f=new bgu.spl.net.Frames.Res.Error("user is not subscribe to this channel or channel does not exist so can not unsubscribe", this.getMessage(), "user is not subscribe to this channel so can not unsubscribe", id);
        else if(!headersCheck(1))
            f = new Error("there are to much headers", this.getMessage(),
            "headers check failed", this.getReciept());
        else{
            c.removeSubscribtion(channelId,id);
            if(this.getReciept()!=-1)
                f=new Reciept(this.getReciept());
        }
        if(f!=null)
            c.send(id, f.toString());
        if(f!=null && f.getClass().equals(Error.class))
            c.disconnect(id);
    }
}
