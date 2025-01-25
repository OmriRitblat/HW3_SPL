package bgu.spl.net.Frames.Res;

import java.net.Authenticator.RequestorType;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Req.RequestFrame;

public class Connected extends ResponseFrame {
    private String version;
    public Connected(String version) {
        super("CONNECTED", Frame.getData("CONNECTED/n version:" + version),"");
        this.version = version;
    }
}
