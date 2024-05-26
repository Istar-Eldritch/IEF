modded class ActionConstructor
{

	override void RegisterActions(TTypenameArray actions)
	{
        #ifdef DIAG_DEVELOPER
        actions.Insert(ActionNameObj);
        #endif
        super.RegisterActions(actions);
	}
};