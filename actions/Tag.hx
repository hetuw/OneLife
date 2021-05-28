package actions;

import sys.FileSystem;
import sys.io.File;
import sys.ssl.Socket;
import haxe.io.BytesOutput;
import haxe.Json;
import haxe.Http;

class Tag
{
    public static function main()
    {
        tagname();
    }
    public static function tagname()
    {
        var r = null;
        var http = new Http("https://api.github.com/repos/jasonrohrer/OneLife/tags");
        http.addHeader("User-Agent","script");
        var output = new BytesOutput();
        var socket = new Socket();
        socket.verifyCert = false;
        http.customRequest(false,output,socket);
        var data = output.getBytes().toString();
        //trace('data $data');
        var array:Array<Data> = Json.parse(data);
        var name = array[0].name;
        Sys.putEnv("last",File.getContent("tag.txt"));
        File.saveContent("tag.txt",name); //save content
        Sys.putEnv("tag",name);
    }
}
private typedef Data = {name:String,zipball_url:String,tarball_url:String,commit:{sha:String,url:String},node_id:String}