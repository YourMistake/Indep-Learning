/*
    rotor.h - C++ Enigma Model (not including middle wheel anomaly)
    ==========================
    Copyright (C) 1997  Andy Carlson (andycarlson@attmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

const int ROTORSIZE=26;
const int ROTORSLOTS=3;
const int ROTORS=5;

// exceptions
class BadWiring {
public:
	BadWiring() {}
};

class Rotor
{
public:
	virtual int translate(int pos) = 0;
	virtual void increment() {}
};

class MovingRotor : public Rotor
{
private:
	int m_position;
	int m_ringSetting;
	int m_notch;
	int m_number;

	int m_fmap[ROTORSIZE];
	int m_rmap[ROTORSIZE];

	Rotor* m_next;
	Rotor* m_prev;

	int relpos(int pos) const;
	int abspos(int pos) const;

	MovingRotor();
public:
	MovingRotor(int num);

	int translate(int pos);
	void increment();

	void indicatedLetter(char pos);
	char indicatedLetter() const;

	void ringSetting(int n);
	int ringSetting() const;

	void loadWiring(char* map);

	void nextRotor(Rotor* next);
	void prevRotor(Rotor* prev);
	int number() const;

	bool atNotch() const;

	void notch(char c) {
		m_notch = c - 'A';
	}
};

class Reflector : public Rotor
{
private:
	int m_map[ROTORSIZE];
public:
	int translate(int pos);

	void loadWiring(char* map);
};

class EntryWheel : public Rotor
{
public:
	int translate(int pos) { return pos; }
};

class Stecker
{
private:
	int m_map[ROTORSIZE];
public:
	Stecker();
	Stecker(const Stecker&);
	void setupAs(const Stecker& s);

	void addWiring(char a, char b);
	void removeWiring(char a);
	void clear();

	int translate(int pos);

	void dump(ostream& o) const;
};

class Scrambler
{
private:
	MovingRotor* m_fitted[ROTORSLOTS];
	int m_isBP;
	int m_nFitted;

	MovingRotor* m_rotor[ROTORS];
	char m_indicator[ROTORSLOTS+1];
	Reflector m_refl;
	EntryWheel m_entry;

	void setup();
public:
	Scrambler();
	Scrambler(const Scrambler&);
	~Scrambler();

	void setupBP();
	void setupAs(const Scrambler&);

	void fitRotor(int n);
	void removeRotors();
	void setIndicator(const char* sz);
	const char* getIndicator() const;

	char translate(char c);
	int translateInt(int i);
	void increment();

	MovingRotor * rotor(int n) const;
	MovingRotor * rotorInSlot(int n) const;

	void dump(ostream& o) const;
};

class Enigma
{
private:
	Scrambler m_scrambler;
	Stecker m_stecker;

public:
	Enigma();
	Enigma(const Enigma&);
	~Enigma();

	void setupAs(const Enigma&);

	void fitRotor(int n);
	void removeRotors();
	void setIndicator(const char* sz);
	const char* Enigma::getIndicator() const;

	char translate(char c);
	char translatePeek(char c);
	void increment();

	void addSteckerWire(char a, char b);
	void removeSteckerWire(char a);
	void clearStecker();

	MovingRotor * rotor(int n) const;
	MovingRotor * rotorInSlot(int n) const;

	Scrambler& scrambler() { return m_scrambler; }

	void dump(ostream& o) const;
};