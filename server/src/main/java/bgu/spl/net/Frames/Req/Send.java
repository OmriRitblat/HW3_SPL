package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Message;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Send extends RequestFrame {
    public Send(String msg) {
        super(-1,msg,"Send");
        String[] lines = msg.split("\n");
        int receipt = -1;
        setRecipet(receipt);
        if(super.getHeaderByKey("destination").equals("") || body == null){
            this.setMissingData(true);
        }
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f;
        if (this.isMissingData()){
            f = new Error("part of the data is missing, please send {destination, send body} in order to SEND", this.getMessage(), "the frame missing data", this.getReciept());
            c.send(id, f);
        }
        else if(!c.isSubscribe(id,super.getHeaderByKey("destination"))){
            f=new Error("user not subscribe to the channel or the cannel does not exist", getMessage(), "user not subscribe to the channel or the cannel does not exist", getReciept());
            c.send(id,f);
        }
        else{
            f=new Message(body);
            c.send(super.getHeaderByKey("destination"), f);
        }
        if(f.getClass().equals(Error.class))
            c.disconnect(id);
    }
}
