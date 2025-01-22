package bgu.spl.net.Frames.Res;

import bgu.spl.net.Frames.Req.RequestFrame;

public class Error extends Reciept {
    private String msg;
    private RequestFrame frame;
    private String details;

    public Error(String msg, String frameMsg, String details,int receipt) {
        super(receipt,"ERROR");
        this.msg = msg;
        this.frame = frame;
        this.details = details;
    }
}
