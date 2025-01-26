package bgu.spl.net.Frames.Res;

import bgu.spl.net.Frames.Req.RequestFrame;

public class Error extends Reciept {
    private String msg;
    private String details;

    public Error(String msg, String frameMsg, String details,int receipt) {
        super("ERROR", ResponseFrame.getData("ERROR\n receipt-id :" + receipt+"\n\n The message:\n ----\n"+msg+"\n ----\n"+details),ResponseFrame.getBody("ERROR/n receipt-id :" + receipt+"\n\n The message:\n ----\n"+msg+"\n ----\n"+details));
        this.msg = msg;
        this.details = details;
    }
}
