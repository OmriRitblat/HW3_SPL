package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Message;
import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class Send extends RequestFrame {

    private String destination;
    public Send(String msg) {
        super(-1,msg,"Send");
        String[] lines = msg.split("\n");
        int receipt = -1;
        setRecipet(receipt);
        if(super.getHeaderByKey("destination").equals("") || body == null){
            this.setMissingData(true);
        }else
            this.destination = super.getHeaderByKey("destination").substring(1);
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f=null;
        if (this.isMissingData()){
            f = new Error("part of the data is missing, please send {destination, send body} in order to SEND", this.getMessage(), "the frame missing data", this.getReciept());
            c.send(id, f.toString());
        }
        else if(!c.isSubscribe(id,destination)){
            f=new Error("user not subscribe to the channel or the cannel does not exist", getMessage(), "user not subscribe to the channel or the cannel does not exist", getReciept());
            c.send(id,f.toString());
        }
        else{
            List<Integer> subToChan=c.getSubsribtion(destination);
            for(Integer i:subToChan){
                f=new Message(body,this.getHeaderByKey("destination"), c.getChannelId(destination,i),c.getMessageCount());
                c.send(i, f.toString());
            }
        }
        if(f.getClass().equals(Error.class))
            c.disconnect(id);
        if (getReciept()!=-1){
            f=new Reciept(getReciept());
            c.send(id, f.toString());
        }
    }

}
