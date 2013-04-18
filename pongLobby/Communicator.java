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
		int id;
		while(_state==LobbyState.WAITING)
		{
			str= _receiveString();
			if(str.split(" ")[0]=="HELLO")
			{
				id= Integer.parseInt(str.split(" ")[1]);
				if(!_isPlayerInVector(id))
					_playerVector.add(new Player(id));
				_sendString("ACK " + id);
			}
		}
		
		for(int i=0;i<5;i++)
			_sendString("GO " + _serverAddress + " " + _serverPort);
	}
	
	
	
	//Methodes privées
	//********************
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
		DatagramPacket packet= new DatagramPacket(msg.getBytes(), msg.length(),
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
	
	private boolean _isPlayerInVector(int id)
	{
		for(int i=0; i<_playerVector.size();i++)
			if(_playerVector.elementAt(i).get_id()==id)
				return false;
		return true;
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
