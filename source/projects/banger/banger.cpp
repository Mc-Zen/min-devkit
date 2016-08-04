/// @file	
///	@ingroup 	minexamples
///	@copyright	Copyright (c) 2016, Cycling '74
/// @author		Andrew Pask, Timothy Place
///	@license	Usage of this file and its contents is governed by the MIT License

#include "c74_min.h"

using namespace c74::min;


class banger : public object<banger> {
public:
	MIN_DESCRIPTION { "Bang at random intervals." };
	MIN_TAGS		{ "time" };
	MIN_AUTHOR		{ "Cycling '74" };
	MIN_RELATED		{ "pedro, link.beat, metro, tempo, drunk" };

	
	inlet	input			{ this, "(toggle) on/off" };
	outlet	bang_out		{ this, "(bang) triggers at randomized interval" };
	outlet	interval_out	{ this, "(float) the interval for the current bang" };


	argument<number> minimum_arg	{ this, "minimum", "Initial lower-bound of generated random interval.",
		MIN_ARGUMENT_FUNCTION {
			min = arg;
		}
	};

	argument<number> maximum_arg	{ this, "maximum", "Initial upper-bound of generated random interval.",
		MIN_ARGUMENT_FUNCTION {
			max = arg;
		}
	};


	timer metro { this, MIN_FUNCTION {
		auto interval = math::random(min, max);
		
		interval_out.send(interval);
		bang_out.send("bang");
		
		metro.delay(interval);
		return {};
	}};


	attribute<double> min { this, "min", 250.0,
		title { "Minimum Interval" },
		description { "Lower-bound of generated random interval." },
		setter { MIN_FUNCTION {
			double value = args[0];
			
			if (value < 1.0)
				value = 1.0;
			return {value};
		}}
	};


	attribute<double> max { this, "max", 1500.0,
		title { "Maximum Interval" },
		description { "Upper-bound of generated random interval." },
		setter { MIN_FUNCTION {
			double value = args[0];
			
			if (value < 1.0)
				value = 1.0;
			return {value};
		}}
	};


	attribute<bool> on { this, "on", false,
		title { "On/Off" },
		description { "Activate the timer." },
		setter { MIN_FUNCTION {
			if (args[0] == true)
				metro.delay(0.0);	// fire the first one straight-away
			else
				metro.stop();
			return args;
		}}
	};
	

	message toggle { this, "int", "Toggle the state of the timer.",
		MIN_FUNCTION {
			on = args[0];
			return {};
		}
	};

};


MIN_EXTERNAL(banger);
