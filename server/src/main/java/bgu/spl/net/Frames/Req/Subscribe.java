package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Subscribe extends RequestFrame {
    private String channelName;
    private int channelId;


    public Subscribe(String msg) {
        super(-1,msg,"Subscribe");
        String[] lines = msg.split("\n");
        int recipt=-1;
        boolean isIdFound=false;
        recipt = Integer.parseInt(super.getHeaderByKey("recipt"));
        channelName = super.getHeaderByKey("destination");
        if(!super.getHeaderByKey("id").equals("")) {
                isIdFound = true;
                channelId = Integer.parseInt(super.getHeaderByKey("id"));
        }
        else{
            isIdFound = false;
        }
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
        else{
            c.addSubscribtion(channelName,id,channelId);
            f=new Reciept(id);
        }
        if(f!=null)
            c.send(id, f);
        if(f!=null && f.getClass().equals(Error.class))
            c.disconnect(id);
    }
}
