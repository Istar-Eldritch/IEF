class ActionNameObj: ActionInteractBase
{
	void ActionNameObj()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Get entity type";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINone;
	}


	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return target != null && target.GetObject() != null;
	}

	override void OnExecuteServer( ActionData action_data )
	{

	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		PlayerBase player = action_data.m_Player;
		GetGame().Chat(action_data.m_Target.GetObject().GetType(), "colorImportant");
	}
	
	override void OnEndServer( ActionData action_data )
	{

	}
}