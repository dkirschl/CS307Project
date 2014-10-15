package cs307.team7.playboiler;

public class User {
	
	private int key;
	private String name;
	private String alias;
	private String gender;
	private int age;
	private String description;
	private String proficiencies;
	
	public User()
	{
		key = -1;
		name = "";
		alias = "";
		gender = "";
		age = 0;
		description = "";
		proficiencies = "";
	}
	
	public User(int k, String n, String a, String g, int ag, String d, String p)
	{
		key = k;
		name = n;
		alias = a;
		gender = g;
		age = ag;
		description = d;
		proficiencies = p;
	}

	public int getKey() {
		return key;
	}

	public void setKey(int key) {
		this.key = key;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getAlias() {
		return alias;
	}

	public void setAlias(String alias) {
		this.alias = alias;
	}

	public String getGender() {
		return gender;
	}

	public void setGender(String gender) {
		this.gender = gender;
	}

	public int getAge() {
		return age;
	}

	public void setAge(int age) {
		this.age = age;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getProficiencies() {
		return proficiencies;
	}

	public void setProficiencies(String proficiencies) {
		this.proficiencies = proficiencies;
	}
}
