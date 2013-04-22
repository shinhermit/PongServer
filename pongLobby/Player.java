package pongLobby;


public class Player {
	Player(long id){
	set_id(id);
	}
	
	//Getters/setters
	//*******************************************
	
	public long get_id() {
		return _id;
	}
	public void set_id(long _id) {
		this._id = _id;
	}
	
	// Privates fields
	//********************************************
	private long _id;
}
