#pragma once
constexpr auto regexStmtNumEntity = "(stmt|read|print|assign|while|if|call)";
constexpr auto regexNameEntity = "(variable|procedure)";
constexpr auto regexEntity = "(procedure|variable|constant|call|assign|stmt|read|print|while|if)";
constexpr auto regexWhileIf = "(while|if)";
constexpr auto regexAssignPrint = "(assign|print)";
constexpr auto regexAssignRead = "(assign|read)";