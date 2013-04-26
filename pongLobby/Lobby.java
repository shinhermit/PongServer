package pongLobby;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Vector;
import java.util.Scanner;

public class Lobby {

	public static void main(String[] args) {
		boolean addressGiven = false;
		Lobby lobby=null;
		InetAddress serverAddress= null;
		lobby = new Lobby(serverAddress);
		lobby.startNetworkThreads();
			
	}
	
	Lobby(InetAddress serverAddress){
		set_state(LobbyState.INITIALISING);
		set_playersVector(new Vector<Player>());
		set_multicastPort(6665);
		set_unicastPort(6664);
		set_serverPort(6666);
		set_serverAddress(serverAddress);
		set_isServerReady(false);
	}
	
	Lobby(InetAddress serverAddress, int unicastPort, int multicastPort, int serverPort)
	{
		set_state(LobbyState.INITIALISING);
		set_playersVector(new Vector<Player>());
		set_unicastPort(unicastPort);
		set_multicastPort(multicastPort);
		set_serverPort(serverPort);
		set_serverAddress(serverAddress);
		set_isServerReady(false);
	}
	
	public void startNetworkThreads(){
		_multicastCommunicator = new MulticastCommunicator(this);
		_unicastListener = new UnicastListener(this);
		_serverListener = new ServerListener(this);
		set_state(LobbyState.WAITING);
		_multicastCommunicator.start();
		_unicastListener.start();
		_serverListener.start();
		_displayMenu();
	}
	
	public String displayPlayers(){
		String string;
		string = "Joueurs connectés à la partie:\n";
		for(int i=0;i<get_playersVector().size();++i)
			string += "Joueur n° " + i + " id: " + get_playersVector().elementAt(i).get_id() + "\n";
		return string;
	}
	
	public Player searchPlayer(long id)
	{
		for(int i=0; i<_playersVector.size(); i++)
			if(_playersVector.elementAt(i).get_id() == id)
				return _playersVector.elementAt(i);
		return null;
	}
	
	public Player searchPlayer(InetAddress ip)
	{
		for(int i=0; i<_playersVector.size(); i++)
			if(_playersVector.elementAt(i).get_ipAddress().equals(ip))
				return _playersVector.elementAt(i);
		return null;
	}
	
	public boolean isPlayerInVector(long id)
	{
		for(int i=0; i<_playersVector.size();i++)
			if(_playersVector.elementAt(i).get_id()==id)
				return true;
		return false;
	}

	
	//Methodes privées
	/*******************/
	private void _displayMenu()
	{
		_quit = false;
		Scanner sc = new Scanner(System.in);
		String str = "";
		while(!_quit)
		{
			while(get_state().equals(LobbyState.WAITING))
			{
				System.out.println("En écoute, " + get_playersVector().size() + " joueurs connectés.");
				System.out.println("a: actualiser cette ligne.");
				System.out.println("l: afficher la liste des joueurs.");
				System.out.println("q: quitter sans lancer la partie.");
				if(get_playersVector().size()>=2)
					System.out.println("s: lancer la partie.");
				str=sc.nextLine();
				if(str.toLowerCase().equals("l"))
					System.out.println(displayPlayers());
				else if(str.toLowerCase().equals("q"))
				{
					set_state(LobbyState.STARTED);
					_quit = true;
				}
				else if(str.toLowerCase().equals("s") && get_playersVector().size()>=2)
				    {
					set_state(LobbyState.STARTING);
					notifyAll();
				    }
			}

			while(get_state().equals(LobbyState.READY_TO_START))
			{
				System.out.println("Lancement du jeu.");
			}
			if(get_state().equals(LobbyState.STARTING))
			{
				System.out.println("Jeu lancé");
				try {
					Thread.currentThread();
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				set_state(LobbyState.WAITING);
			}
			if(get_state().equals(LobbyState.STARTED))
			{
				System.out.println("Lobby fermé.");
				System.exit(0);
			}
		}
		sc.close();
	}
	
	//getters/setters
	/******************/
	public LobbyState get_state() {
		return _state;
	}

	public void set_state(LobbyState _state) {
		this._state = _state;
	}


	public Vector<Player> get_playersVector() {
		return _playersVector;
	}

	public void set_playersVector(Vector<Player> _playersVector) {
		this._playersVector = _playersVector;
	}


	public int get_multicastPort() {
		return _multicastPort;
	}

	public void set_multicastPort(int _multicastPort) {
		this._multicastPort = _multicastPort;
	}


	public int get_unicastPort() {
		return _unicastPort;
	}

	public void set_unicastPort(int _unicastPort) {
		this._unicastPort = _unicastPort;
	}

	public String get_localAddress()
	{
		String address= "";
		try {
			address= Inet4Address.getLocalHost().getHostAddress();
		} catch (UnknownHostException e) {
			System.out.println(" Impossible d'obtrenir l'addresse locale: " + e);
		}
		return address;
	}

	public int get_serverPort() {
		return _serverPort;
	}

	public void set_serverPort(int _serverPort) {
		this._serverPort = _serverPort;
	}

	public InetAddress get_serverAddress() {
		return _serverAddress;
	}

	public void set_serverAddress(InetAddress _serverAddress) {
		this._serverAddress = _serverAddress;
	}

	public boolean is_isServerReady() {
		return _isServerReady;
	}

	public void set_isServerReady(boolean _isServerReady) {
		this._isServerReady = _isServerReady;
	}

	private boolean _quit;
	private Vector<Player> _playersVector;
	private LobbyState _state;
	private MulticastCommunicator _multicastCommunicator;
	private UnicastListener _unicastListener;
	private ServerListener _serverListener;
	private int _multicastPort;
	private int _unicastPort;
	private int _serverPort;
	private InetAddress _serverAddress;
	private boolean _isServerReady;

}
