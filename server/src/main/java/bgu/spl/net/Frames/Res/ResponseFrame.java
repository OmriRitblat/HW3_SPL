package bgu.spl.net.Frames.Res;

import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Req.RequestFrame;

public abstract class ResponseFrame extends Frame {
    
    public ResponseFrame(String type, ConcurrentHashMap<String, String> headers, String body) {
        super(type,headers,body);
    }
}
