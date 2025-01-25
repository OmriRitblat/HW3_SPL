package bgu.spl.net.Frames.Req;

import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.Frames.Res.Connected;
import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Connect extends RequestFrame {
    private String version;
    private String host;
    private String user;
    private String password;

    public Connect(String msg) {
        super(-1, msg, "Connect");
        String[] lines = msg.split("\n");
        int recipt = -1;
        ConcurrentHashMap<String, String> data = super.getHeaders();
        if (data.containsKey("receipt"))
            recipt = Integer.parseInt(data.get("receipt"));
        if (data.containsKey("host"))
            host = data.get("host");
        if (data.containsKey("login"))
            user = data.get("login");
        if (data.containsKey("passcode"))
            password = data.get("passcode");
        if (data.containsKey("accept-version"))
            version = data.get("accept-version");
        this.setRecipet(recipt);
        if (version == null || host == null || user == null || password == null)
            setMissingData(true);
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f;
        if (this.isMissingData()){
            f = new Error("part of the data is missing, please send {version, host, user, password} in order to CONNECT",
                    this.getMessage(), "the frame missing data", this.getReciept());}
        else if (c.isLoggedIn(id) || !c.isCorrectPassword(id, password))
            f = new Error("User already logged in or password is not match", this.getMessage(),
                    "User already logged in or password is not match", this.getReciept());
        else if (!version.equals("1.2"))
            f = new Error("the version does not match the latest version, should be 1.2", this.getMessage(),
                    "version does not match", this.getReciept());
        else {
            c.login(id);
            f = new Connected(version);
            if (this.getReciept() != -1) {
                c.send(id, f.toString());
                f = new Reciept(getReciept());
            }
        }
        c.send(id, f.toString());
        if (f.getClass().equals(Error.class))
            c.disconnect(id);

    }
}
