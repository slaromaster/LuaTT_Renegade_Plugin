#pragma once

#ifndef AIDIALOGUE_INCLUDE_CONVERSATIONCONTROL_H
#define AIDIALOGUE_INCLUDE_CONVERSATIONCONTROL_H

enum DialogueType
{
	TAKE_DAMAGE_FROM_FRIEND = 0,
	TAKE_DAMAGE_FROM_ENEMY,
	DAMAGE_FRIEND,
	DAMAGE_ENEMY,
	KILLED_FRIEND,
	KILLED_ENEMY,
	SAW_FRIEND,
	SAW_ENEMY,
	OBSOLETE_01,
	OBSOLETE_02,
	DIE,
	POKE_IDLE,
	POKE_SEARCH,
	POKE_COMBAT,
	IDLE_TO_COMBAT,
	IDLE_TO_SEARCH,
	SEARCH_TO_COMBAT,
	SEARCH_TO_IDLE,
	COMBAT_TO_SEARCH,
	COMBAT_TO_IDLE,
	DIALOGUE_TYPE_NONE
};


class DialogueClass;

class ConversationControl
{
	public:
		static ConversationClass *Find_Conversation(int ConversationID);
		static ConversationClass *Find_Conversation(const char *Conversation_Name); // Does not work?
};

class AI_Dialogue_Script : public ScriptImpClass
{
	public:
		enum AIDialogueCustom
		{
			CUSTOM_EVENT_DAMAGED_ENEMY = 53548,
			CUSTOM_EVENT_DAMAGED_FRIEND,
			CUSTOM_EVENT_KILLED_ENEMY,
			CUSTOM_EVENT_KILLED_FRIEND
		};

		virtual void Created(GameObject *obj);
		virtual void Damaged(GameObject *obj, GameObject *damager, float amount);
		virtual void Killed(GameObject *obj,GameObject *killer);
		virtual void Poked(GameObject *obj, GameObject *poker);
		virtual void Timer_Expired(GameObject *obj, int number);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Enemy_Seen(GameObject *obj, GameObject *enemy);
		void State_Changed(SoldierAIState From, SoldierAIState To);
		SoldierAIState Get_Current_State();
		void Act(DialogueType Type);
		DialogueClass &Get_Dialogue(DialogueType Type);
		ConversationClass *Get_Random_Conversation(DialogueClass &Dialogue);

	private:
		bool _Acting;
		SoldierAIState _LastState;
		bool _SawEnemy;
		bool _SawFriend;
		bool _EnemyKilled;
};

#endif