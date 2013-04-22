package pongLobby;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Vector;
import java.util.Scanner;

public class Lobby {

	public static void main(String[] args) {
		boolean addressGiven = false;
		Lobby lobby=null;
		InetAddress serverAddress= null;
		int serverPort=0, localPort=0;
		int nbParameter=0;
		for(int i=0;i<args.length;i+=2)
		{
			if(args[i].equals("-h")){
				try{
					serverAddress = InetAddress.getByName(args[i+1]);
				}
				catch(UnknownHostException ex)
				{
					System.out.println("Addresse du serveur inconnue: " + ex);
					System.exit(1);
				}
				addressGiven=true;
			}
			else if(args[i].equals("-lp")){
				localPort=Integer.parseInt(args[i+1]);
				nbParameter+=1;
			}
			else if(args[i].equals("-sp")){
				serverPort=Integer.parseInt(args[i+1]);
				nbParameter+=1;
			}
			else
			{
				System.out.println("Argument " + args[i] + " inconnu.");
			}
			
		}
		
		if(!addressGiven || (nbParameter==1))
		
		{
			System.out.println("Merci d'utiliser le logiciel comme suit:");
			System.out.println("-h pour spécifier l'addresse ou le nom du serveur de jeu");
			System.out.println("-lp pour spécifier le port du lobby");
			System.out.println("-sp pour spécifier le port du serveur de jeu");			
			System.out.println("merci de spécifier au moins l'addresse du serveur de jeu");
		}
		else{
			if(nbParameter==2)
				lobby = new Lobby(serverAddress, localPort, serverPort);
			else
				lobby = new Lobby(serverAddress);
			lobby.startNetworkThread();
		}
			
	}
	
	Lobby(InetAddress serverAddress){
		_state=LobbyState.INITIALISING;
		_playersVector = new Vector<Player>();
		_port = 6665;
		_serverPort = 6666;
		_serverAddress= serverAddress;
	}
	
	Lobby(InetAddress serverAddress, int port, int serverPort)
	{
		_state=LobbyState.INITIALISING;
		_playersVector = new Vector<Player>();
		_port = port;
		_serverPort = serverPort;
		_serverAddress= serverAddress;
	}
	
	public void startNetworkThread(){
		_communicator = new Communicator(_playersVector, _state, _port, _serverAddress, _serverPort);
		_communicator.start();
		_displayMenu();
	}
	
	public String displayPlayers(){
		String string;
		string = "Joueurs connectés à la partie:\n";
		for(int i=0;i<_playersVector.size();++i)
			string += "Joueur n° " + i + " id: " + _playersVector.elementAt(i).get_id() + "\n";
		return string;
	}
	
	private void _displayMenu()
	{
		Scanner sc = new Scanner(System.in);
		String str = "";
		while(_state.equals(LobbyState.INITIALISING))
		{
			System.out.println("En écoute, " + _playersVector.size() + " joueurs connectés.");
			System.out.println("a: actualiser cette ligne.");
			System.out.println("l: afficher la liste des joueurs.");
			System.out.println("q: quitter sans lancer la partie.");
			if(_playersVector.size()>=2)
				System.out.println("s: lancer la partie.");
			str=sc.nextLine();
			if(str.toLowerCase().equals("l"))
				System.out.println(displayPlayers());
			else if(str.toLowerCase().equals("q"))
				_state=LobbyState.STARTED;
			else if(str.toLowerCase().equals("s") && _playersVector.size()>=2)
				_state=LobbyState.READY_TO_START;
		}
		
		while(_state.equals(LobbyState.READY_TO_START))
		{
			System.out.println("Lancement du jeu.");
		}
		if(_state.equals(LobbyState.STARTING))
			System.out.println("Jeu lancé, fermeture du serveur du lobby.");
		while(_state.equals(LobbyState.STARTING))
		{

		}
		if(_state.equals(LobbyState.STARTED))
		{
			System.out.println("Lobby fermé.");
			System.exit(0);
		}
	}
	
	
	private Vector<Player> _playersVector;
	private LobbyState _state;
	private Communicator _communicator;
	private int _port;
	private int _serverPort;
	private InetAddress _serverAddress;

}
