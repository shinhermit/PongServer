package pongLobby;

import java.lang.Thread;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;
import java.net.DatagramPacket;
import java.io.IOException;



public class MulticastCommunicator extends Thread {
	MulticastCommunicator(Lobby lobby){
		_lobby=lobby;
	}
	
	
	//Methodes publiques
	//********************
	public void run()
	{	
		_initializeSocket();
		//Lancement de la routine de scrutations de nouveaux joueurs

		String str;
		long id=0;
		int port=0;
		InetAddress address = null;
		while(_lobby.get_state().equals(LobbyState.WAITING))
		{
			str="";
			str= _receiveString();
			if(str.split(" ")[0].equals("HELLO"))
			{
				try{
					id= Long.parseLong(str.split(" ")[1]);
					port= Integer.parseInt(str.split(" ")[2]);
				}
				catch(NumberFormatException ex)
				{
					System.out.println(ex);
					System.exit(1);
				}
				try{
					address = InetAddress.getByName(str.split(" ")[3]);
				}
				catch(UnknownHostException exception)
				{
					System.out.println("Impossible de trouver l'adresse ip: " + exception);
					System.exit(1);
				}
				if(!_lobby.isPlayerInVector(id))
				{

					_lobby.get_playersVector().add(new Player(id, port, address, _lobby));
					_sendString("SERVER " + id + " " + _lobby.get_unicastPort() + " " + _lobby.get_localAddress());
					System.out.println("Un joueur s'est connecté.");
				}
			}
		}
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
			_socket = new MulticastSocket(_lobby.get_multicastPort());
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
				_multicastGroup,_lobby.get_multicastPort());
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
				_multicastGroup, _lobby.get_multicastPort());
		try{
			_socket.receive(packet);
		}
		catch(IOException exception)
		{
			System.out.println("Impossible de lire le paquet dans la socket: "
					+ exception);
		}
		String str = new String(packet.getData(), 0, packet.getLength());
		System.out.println(str);
		return str;
	}

	
	
	
	//Membres privés
	//********************
	private Lobby _lobby;
	private InetAddress _multicastGroup;
	private MulticastSocket _socket;
}
