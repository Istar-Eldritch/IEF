
## Liquid Framework


### Using the `IE\Liquids.json` Configuration

You can use the configuration to resolve liquids added by other mods showing as `ERROR`. You need to know the id of the liquid in question.
In this example we'll modify the liquid `Beer` that exists by default in vanilla, and we'll change its name & color. We'll name it `Nitric Acid` and we'll color it green. Note that this doesn't change the liquid properties, its just a cosmetic change, mainly used to resolve the forementioned issue that arises when adding multiple mods with liquids.

```js
// Vanilla liquid ids
LIQUID_BLOOD_0_P = 1;
LIQUID_BLOOD_0_N = 2;
LIQUID_BLOOD_A_P = 4;
LIQUID_BLOOD_A_N = 8;
LIQUID_BLOOD_B_P = 16;
LIQUID_BLOOD_B_N = 32;
LIQUID_BLOOD_AB_P = 64;
LIQUID_BLOOD_AB_N = 128;
LIQUID_SALINE  = 256;

LIQUID_WATER = 512;
LIQUID_RIVERWATER = 1024;
LIQUID_VODKA = 2048;
LIQUID_BEER = 4096;
LIQUID_GASOLINE = 8192;
LIQUID_DIESEL = 16384;
LIQUID_DISINFECTANT = 32768;
```

As you can see the beer liquid has id `4096`.

To color it, we'll first find hex code for the color we like for example [`#32cd32`](https://www.colorhexa.com/32cd32), then we'll get its decimal representation using a [conversion tool](https://www.rapidtables.com/convert/number/hex-to-decimal.html). The final color code we get is `3329330` in decimal.

Finally we add the liquid to `IE\Liquids.json`

```json
{
    "version": 1,
    "liquids": [
        {
            "id": 4096,
            "name": "Nitric Acid",
            "color": 3329330
        }
    ]
}
```


### Using the `IE_LiquidRegister` class.

You can add liquid compatibility directly by using the LiquidRegister. This also allows you to define custom logic that applies to your liquid in a way that is not possible using the configuration. When adding new liquids, the vanilla implementation uses powers of two for its liquid types. The reason for this is because the bit position is used as a flag to identify which liquids can fit specific bottles.
If you check for `WaterBottle` you will find:
```c
liquidContainerType="1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) -32768";
```
This rather complicated list basically says that water bottles can contain:
```js
LIQUID_WATER = 512;
LIQUID_RIVERWATER = 1024;
LIQUID_VODKA = 2048;
LIQUID_BEER = 4096;
LIQUID_GASOLINE = 8192;
LIQUID_DIESEL = 16384;
```

If you transform the previous numbers to binary you will see:

```js
LIQUID_WATER =      0000001000000000;
LIQUID_RIVERWATER = 0000010000000000;
LIQUID_VODKA =      0000100000000000;
LIQUID_BEER =       0001000000000000;
LIQUID_GASOLINE =   0010000000000000;
LIQUID_DIESEL =     0100000000000000;
```

If you want to add a custom liquid, and you want it to fit a water bottle, you don't have to create it with a power of two liquid type, just ensure one of the previous bits is active. For example:

```js
LIQUID_WATER =  0000001000000000;
LIQUID_CUSTOM = 0000001000010000;
```

And then you get the decimal version of it you will get a liquid type that is valid and will get in all containers that can contain water:
```js
LIQUID_CUSTOM = 528;
```

To register your custom liquid, you need to register your liquid on your `config.cfg` as usual:

```cpp
class cfgLiquidDefinitions
{
	class CustomLiquid
	{
		type=528;
		displayName="My Custom Liquid";
		flammability=60;
		class Nutrition
		{
			fullnessIndex=1;
			energy=-10;
			water=0;
			nutritionalIndex=75;
			toxicity=1;
			digestibility=2;
			agents=16;
		};
	};
};
```

Create a class to define your liquid extending from `IE_LiquidDetailsBase`

```cpp
class CustomLiquid : IE_LiquidDetailsBase
{
    override string GetName(ItemBase item)
    {
        if (item)
        {
            if (item.IsFrozen())
            {
                return "Frozen custom liquid";
            }
        }
        return "Custom Liquid";
    }

    override int GetColor(ItemBase item)
    {
        if (item)
        {
            if (item.IsFrozen())
            {
                return COLOR_ICE;
            }
        }
        return Colors.COLOR_LIQUID;
    }
}
```

And register it on the LiquidRegistry:

```cpp
const int LIQUID_CUSTOM = 528;

modded class MissionBase
{
    void MissionBase()
    {
        auto registry = IE_GetLiquidRegistry();
        registry.RegisterLiquid(LIQUID_CUSTOM, new CustomLiquid());
    }
}
```