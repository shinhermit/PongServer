package pongLobby;

import java.net.InetAddress;
import javax.swing.Timer;


public class Player {
	Player(long id, int port, InetAddress ipAddress, Lobby lobby){
	set_id(id);
	set_port(port);
	set_ipAddress(ipAddress);
	_lobby=lobby;
	_timer = new Timer(500, new PlayerTimeout(this));
	_timer.start();
	}
	
	public void stopTimer()
	{
		_timer.stop();
	}
	
	public void resetTimer()
	{
		_timer.restart();
	}
	//Getters/setters
	//*******************************************
	
	public long get_id() {
		return _id;
	}
	public void set_id(long _id) {
		this._id = _id;
	}
	
	public int get_port() {
		return _port;
	}

	public void set_port(int port) {
		this._port = port;
	}

	public InetAddress get_ipAddress() {
		return _ipAddress;
	}

	public void set_ipAddress(InetAddress _ipAddress) {
		this._ipAddress = _ipAddress;
	}

	public Lobby get_lobby() {
		return _lobby;
	}

	public void set_lobby(Lobby _lobby) {
		this._lobby = _lobby;
	}

	// Privates fields
	//********************************************
	private long _id;
	private InetAddress _ipAddress;
	private int _port;
	private Timer _timer;
	private Lobby _lobby;
}
