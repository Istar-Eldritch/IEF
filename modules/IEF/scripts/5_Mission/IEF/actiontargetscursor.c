

modded class ActionTargetsCursor
{

    ref array<ref Widget> m_BlockingIcons = new array<ref Widget>;
    ref array<string> m_ActionTypes = new array<string>;

    override protected void OnWidgetScriptInit(Widget w)
    {
        super.OnWidgetScriptInit(w);

        m_ActionTypes.Insert("interact");
        m_ActionTypes.Insert("continuous_interact");
        m_ActionTypes.Insert("single");
        m_ActionTypes.Insert("continuous");

        CreateBlockingIcons("IE/Framework/data/layouts/ie_framework_blocked_icon.layout", "btn_icon_blocked");
    }

    protected void CreateBlockingIcons(string layout, string name)
    {
		foreach (string action : m_ActionTypes)
        {
            Widget existingIcon = m_Root.FindAnyWidget(action + "_btn_icon");
            Widget blockedIconWidget = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets(layout, existingIcon.GetParent()));
			blockedIconWidget.SetName(action + "_" +  name);
			blockedIconWidget.Show(false);

            float x;
            float y;
            existingIcon.GetScreenPos(x, y);
            blockedIconWidget.SetScreenPos(x, y);
            float width;
            float height;
            existingIcon.GetScreenSize(width, height);
            blockedIconWidget.SetScreenSize(width, height);
            m_BlockingIcons.Insert(blockedIconWidget);
        }
    }

	override protected void SetActionWidget(ActionBase action, string descText, string actionWidget, string descWidget)
	{
		Widget widget = m_Root.FindAnyWidget(actionWidget);
		
		if (action)
		{
			if (action.HasTarget() && m_AM.GetActionState() < 1) // targeted & action not performing
			{
                TextWidget actionName = TextWidget.Cast(widget.FindAnyWidget(descWidget));
                Widget actionIcon = widget.FindAnyWidget(actionWidget + "_btn_icon");
                string blockedIconId = action.GetBlockedIcon(m_Player, m_Target, m_Player.GetItemInHands());
                Widget blockedIconWidget = actionIcon.GetParent().FindAnyWidget(actionWidget + "_" + blockedIconId); 

                bool isBlocked = action.IsBlocked(m_Player, m_Target, m_Player.GetItemInHands());
				if (!isBlocked && action.GetInput().GetInputType() == ActionInputType.AIT_CONTINUOUS)
				{
					descText = descText + " " + "#action_target_cursor_hold";
					actionName.SetText(descText);
                    actionIcon.Show(true);
                    foreach (Widget icon : m_BlockingIcons)
                    {
                        icon.Show(false);
                    }
				}
				else
				{
                    if (isBlocked)
                    {
                        action.GetBlockedText(m_Player, m_Target, m_Player.GetItemInHands());
                        actionName.SetText(action.GetBlockedText(m_Player, m_Target, m_Player.GetItemInHands()));
                        actionIcon.Show(false);
                        if (blockedIconWidget)
                        {
                            blockedIconWidget.Show(true);
                        }
                    }
                    else
                    {
                        actionIcon.Show(true);
                        actionName.SetText(descText);
                        if (blockedIconWidget)
                        {
                            blockedIconWidget.Show(false);
                        }
                    }
				}

				widget.Show(true);
				
				int x, y;
				actionName.GetTextSize(x, y);
				if (x > m_MaxWidthChild);
					m_MaxWidthChild = x;
			}
			else
			{
				widget.Show(false);
			}
		}
		else
		{
			widget.Show(false);
		}
	}
}