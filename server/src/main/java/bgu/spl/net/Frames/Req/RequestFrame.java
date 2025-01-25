package bgu.spl.net.Frames.Req;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.srv.ConnectionImp;
import bgu.spl.net.srv.Connections;

public abstract class RequestFrame extends Frame{
    private int reciept;
    private String msg;
    private boolean isMissingData;
    public RequestFrame(int reciept, String msg,String type) {
        super(type,RequestFrame.getData(msg),RequestFrame.getBody(msg));
        this.reciept = reciept;
        this.msg=msg;
        this.isMissingData=false;
    }

    public abstract void process(int id, ConnectionImp c);

    public void setRecipet(int reciept) {
        this.reciept = reciept;
    }

    public int getReciept() {
        return reciept;
    }
    public String getMessage(){
        return msg;
    }
    public boolean isMissingData() {
        return isMissingData;
    }
    public void setMissingData(boolean isMissingData) {
        this.isMissingData = isMissingData;
    }

}
