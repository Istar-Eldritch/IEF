modded class PlayerBase
{
	override void SetActions(out TInputActionMap InputActionMap)
	{
		AddAction(ActionNameObj, InputActionMap);
        super.SetActions(InputActionMap);
	}
}