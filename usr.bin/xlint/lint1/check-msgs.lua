#! /usr/bin/lua
-- $NetBSD: check-msgs.lua,v 1.10 2021/04/14 18:35:40 rillig Exp $

--[[

usage: lua ./check-msgs.lua *.c

Check that the message text in the comments of the C source code matches the
actual user-visible message text in err.c.

]]


local function load_messages(fname)
  local msgs = {}

  local f = assert(io.open(fname, "r"))
  for line in f:lines() do
    local msg, id = line:match("%s*\"(.+)\",%s*/%*%s*(%d+)%s*%*/$")
    if msg ~= nil then
      msgs[tonumber(id)] = msg
    end
  end

  f:close()

  return msgs
end


local function check_message(fname, lineno, id, comment, msgs, errors)
  local msg = msgs[id]

  if msg == nil then
    errors:add("%s:%d: id=%d not found", fname, lineno, id)
    return
  end

  msg = msg:gsub("/%*", "**")
  msg = msg:gsub("%*/", "**")
  msg = msg:gsub("\\(.)", "%1")

  comment = comment:gsub("arg%.", "argument")
  comment = comment:gsub("comb%.", "combination")
  comment = comment:gsub("conv%.", "conversion")
  comment = comment:gsub("decl%.", "declaration")
  comment = comment:gsub("defn%.", "definition")
  comment = comment:gsub("des%.s", "designators")
  comment = comment:gsub("expr%.", "expression")
  comment = comment:gsub("func%.", "function")
  comment = comment:gsub("incomp%.", "incompatible")
  comment = comment:gsub("init%.", "initialize")
  comment = comment:gsub("param%.", "parameter")
  comment = comment:gsub("req%.", "requires")
  comment = comment:gsub("poss%.", "possibly")
  comment = comment:gsub("trad%.", "traditional")

  if comment == msg then
    return
  end

  local prefix = comment:match("^(.-)%s*%.%.%.$")
  if prefix ~= nil and msg:find(prefix, 1, 1) == 1 then
    return
  end

  errors:add("%s:%d:   id=%-3d   msg=%-40s   comment=%s",
    fname, lineno, id, msg, comment)
end


local function collect_errors(fname, msgs)
  local errors = {}
  errors.add = function(self, fmt, ...)
    table.insert(self, fmt:format(...))
  end

  local f = assert(io.open(fname, "r"))
  local lineno = 0
  local prev = ""
  for line in f:lines() do
    lineno = lineno + 1

    local func, id = line:match("^%s+(%w+)%((%d+)[),]")
    id = tonumber(id)
    if func == "error" or func == "warning" or
       func == "c99ism" or func == "c11ism" or
       func == "gnuism" or func == "message" then
      local comment = prev:match("^%s+/%* (.+) %*/$")
      if comment ~= nil then
        check_message(fname, lineno, id, comment, msgs, errors)
      else
        errors:add("%s:%d: missing comment for %d: /* %s */",
          fname, lineno, id, msgs[id])
      end
    end

    prev = line
  end

  f:close()

  return errors
end


local function check_file(fname, msgs)
  local errors = collect_errors(fname, msgs)
  for _, err in ipairs(errors) do
    print(err)
  end
  return #errors == 0
end

local function file_contains(filename, text)
  local f = assert(io.open(filename, "r"))
  local found = f:read("a"):find(text, 1, true)
  f:close()
  return found
end

local function check_test_files(msgs)

  local msgids = {}
  for msgid, _ in pairs(msgs) do
    table.insert(msgids, msgid)
  end
  table.sort(msgids)

  local testdir = "../../../tests/usr.bin/xlint/lint1"
  local ok = true
  for _, msgid in ipairs(msgids) do
    local msg = msgs[msgid]:gsub("\\(.)", "%1")
    local filename = ("%s/msg_%03d.c"):format(testdir, msgid)
    if not file_contains(filename, msg) then
      ok = false
      print(("%s must contain: %s"):format(filename, msg))
    end
  end

  return ok
end

local function main(arg)
  local msgs = load_messages("err.c")
  local ok = true
  for _, fname in ipairs(arg) do
    ok = check_file(fname, msgs) and ok
  end
  ok = check_test_files(msgs) and ok
  return ok
end


os.exit(main(arg))
