# Blocked Actions

A blocked action requires implementing the methods `IsBlocked`, `GetBlockedText` and `GetBlockedIcon` on your action, as well as optionally provide a custom icon.

## Implementing the methods

The following excerpt is taken from ThermalSystem and shows the DrainLiquid action being blocked when the item is frozen.

```c#
modded class ActionDrainLiquid
{
    // Return true when the action is blocked
    override bool IsBlocked(PlayerBase player, ActionTarget target, ItemBase item)
    {
        // This is required so if some other mod blocked this action for some reason
        // they have the chance to do so.
        bool isBlocked = super.IsBlocked(player, target, item);
        if (!isBlocked)
        {
            ItemBase targetItem = ItemBase.Cast(target.GetObject());
            return targetItem != null && targetItem.IsFrozen();
        }
        return isBlocked;
    }

    // Returns the text showed in the UI when the icon is blocked.
    // There is no default implementation, if you block an action you have to return some
    // custom text here.
    //
    // If you are not blocking the action, return an empty string so others can block
    // the action for different reasons.
    override string GetBlockedText(PlayerBase player, ActionTarget target, ItemBase item)
    {
        // This is required so if some other mod blocked this action for some reason
        // they have the chance to show their custom text.
        string blockedText = super.GetBlockedText(player, target, item);
        if (blockedText)
            return blockedText;

        ItemBase targetItem = ItemBase.Cast(target.GetObject());
        if(targetItem != null && targetItem.IsFrozen())
        {
            string drainFrozenLoc = Widget.TranslateString( "#STR_IETS_ACTION_DRAIN_FROZEN");
            return drainFrozenLoc;
        }

        return "";
    }

    // Returns the icon showed in the UI when the action is blocked.
    // There is no default implementation, if you block an action and you don't have a custom icon
    // use `btn_icon_blocked`
    //
    // If you are not blocking the action, return an empty string so others can block
    // the action for different reasons.
    override string GetBlockedIcon(PlayerBase player, ActionTarget target, ItemBase item)
    {
        // This is required so if some other mod blocked this action for some reason
        // they have the chance to show their custom icon.
        string blockedIcon = super.GetBlockedIcon(player, target, item);
        if (blockedIcon)
            return blockedIcon;

        ItemBase targetItem = ItemBase.Cast(target.GetObject());
        if(targetItem != null && targetItem.IsFrozen())
        {
            return "btn_icon_blocked";
        }
        
        return "";
    }
};
```


## Adding a custom icon.


First of all, import the icon using workbench. The icon must be in `.edds` format.

Create a new `.layout` file

```c#
// Rename the widget to your liking
ImageWidgetClass btn_icon_custom_blocked {
 position 10 10
 size 100 100
 hexactpos 1
 vexactpos 1
 hexactsize 1
 vexactsize 1
 imageTexture "{EB15A64881430D36}IEF/data/images/blocked_icon.edds" // This should be referencing your .edds
 mode blend
 "src alpha" 1
 "clamp mode" wrap
 "stretch mode" stretch_w_h
}
```

Register the actions on `ActionTargetsCursor` and `ItemActionsWidget`

```c#
modded class ActionTargetsCursor
{
    override protected void OnWidgetScriptInit(Widget w)
    {
        super.OnWidgetScriptInit(w);
        CreateBlockingIcons("<MOD_PREFIX>/custom_blocked_icon.layout", "btn_icon_custom_blocked");
    }
}

modded class ItemActionsWidget
{
    override protected void OnWidgetScriptInit(Widget w)
    {
        super.OnWidgetScriptInit(w);
        CreateBlockingIcons("<MOD_PREFIX>/custom_blocked_icon.layout", "btn_icon_custom_blocked");
    }
}
```

You can now return your custom icon name from the `GetBlockedIcon` implementation:

```c#
modded class YourBlockedAction
{
    override string GetBlockedIcon(PlayerBase player, ActionTarget target, ItemBase item)
    {
        return "btn_icon_custom_blocked";
    }
};
```