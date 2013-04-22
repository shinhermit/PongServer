package pongLobby;

import java.lang.Thread;
import java.util.Vector;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;
import java.net.DatagramPacket;
import java.io.IOException;



public class Communicator extends Thread {
	Communicator(Vector<Player> playerVector, LobbyState state, int port, InetAddress serverAddress, int serverPort){
		_playerVector=playerVector;
		_state=state;
		_port = port;
		_serverAddress=serverAddress;
		_serverPort=serverPort;
	}
	
	
	//Methodes publiques
	//********************
	public void run()
	{	
		_initializeSocket();
		//Lancement de la routine de scrutations de nouveaux joueurs
		_state=LobbyState.WAITING;
		
		String str;
		long id;
		while(_state.equals(LobbyState.WAITING))
		{
			str="";
			str= _receiveString();
			if(str.split(" ")[0].equals("HELLO"))
			{
				try{
					id= Long.parseLong(str.split(" ")[1]);
					if(!_isPlayerInVector(id))
					{
						System.out.println("Un joueur s'est connecté.");
						_playerVector.add(new Player(id));
					}
					_sendString("ACK " + id);
				}
				catch(NumberFormatException ex)
				{
					System.out.println(ex);
					System.exit(1);
				}
			}
			else if(str.split(" ")[0].equals("BYE"))
			{
				try{
					id= Long.parseLong(str.split(" ")[1]);
					if(_isPlayerInVector(id))
					{
						System.out.println("Un joueur s'est déconnecté.");
						_playerVector.remove(_searchPlayer(id));
					}
				}
				catch(NumberFormatException ex)
				{
					System.out.println(ex);
					System.exit(1);
				}
			}
		}
		
		if(_state.equals(LobbyState.READY_TO_START))
		{
			for(int i=0;i<5;i++)
				_sendString("GO " + _serverAddress + " " + _serverPort);
		}
		_state=LobbyState.STARTING;
		_socket.close();
		_state=LobbyState.STARTED;
	}
	
	
	//Methodes privées
	//********************
	private Player _searchPlayer(long id)
	{
		for(int i=0; i<_playerVector.size(); i++)
			if(_playerVector.elementAt(i).get_id() == id)
				return _playerVector.elementAt(i);
		return null;
	}
	
	private void _initializeSocket()
	{
		try{
			_multicastGroup= InetAddress.getByName("228.5.6.7");
		}
		catch(UnknownHostException exception)
		{
			System.out.println("Impossible de déterminer le groupe: " + exception);
			System.exit(1);
		}
		
		try{
			_socket = new MulticastSocket(_port);
			_socket.joinGroup(_multicastGroup);
		}
		catch(IOException exception)
		{
			System.out.println("Impossible de lancer la socket: " + exception);
			System.exit(1);
		}
	}
	
	private void _sendString(String msg){
		String str=msg;
		str+="\0";
		DatagramPacket packet= new DatagramPacket(str.getBytes(), str.length(),
				_multicastGroup,_port);
		try{
		_socket.send(packet);
		}
		catch(IOException exception)
		{
			System.out.println("Impossible d'envoyer le paquet dans la socket: "
					+ exception);
			System.exit(1);
		}
	}
	
	private String _receiveString(){
		byte[] data = new byte[200];
		DatagramPacket packet = new DatagramPacket(data, data.length, 
				_multicastGroup, _port);
		try{
			_socket.receive(packet);
		}
		catch(IOException exception)
		{
			System.out.println("Impossible de lire le paquet dans la socket: "
					+ exception);
		}
		String str = new String(packet.getData(), 0, packet.getLength());
		return str;
	}
	
	private boolean _isPlayerInVector(long id)
	{
		for(int i=0; i<_playerVector.size();i++)
			if(_playerVector.elementAt(i).get_id()==id)
				return true;
		return false;
	}
	
	
	
	//Membres privés
	//********************
	private LobbyState _state;
	private MulticastSocket _socket;
	private InetAddress _multicastGroup;
	private Vector<Player> _playerVector;
	private int _port;
	private InetAddress _serverAddress;
	private int _serverPort;

}
