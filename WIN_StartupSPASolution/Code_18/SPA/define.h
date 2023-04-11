#pragma once
constexpr auto regexStmtNumEntity = "(stmt|read|print|assign|while|if|call)";
constexpr auto regexStmtNumEntityNoStmt = "(read|print|assign|while|if|call)";
constexpr auto regexNameEntity = "(variable|procedure)";
constexpr auto regexEntity = "(procedure|variable|constant|call|assign|stmt|read|print|while|if)";
constexpr auto regexWhileIf = "(while|if)";
constexpr auto regexAssignPrint = "(assign|print)";
constexpr auto regexAssignRead = "(assign|read)";
constexpr auto regexWord = "\\w+";
constexpr auto regexQuote = "\\\"";
constexpr auto regexVariables = "^((?!(procedure|while|if|then|else|call|read|print)$)[A-Za-z][A-Za-z0-9]*)";
constexpr auto regexConstants = "^[0-9]+$";