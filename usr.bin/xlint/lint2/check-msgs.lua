#! /usr/bin/lua
-- $NetBSD: check-msgs.lua,v 1.1 2021/02/28 18:17:08 rillig Exp $

--[[

usage: lua ./check-msgs.lua *.c

Check that the message text in the comments of the C source code matches the
actual user-visible message text in msg.c.

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
  msg = msg:gsub("\\t", "\\\\t")  -- for lint2
  msg = msg:gsub("\\(.)", "%1")

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
    if func == "msg" then
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


local function main(arg)
  local msgs = load_messages("msg.c")
  local ok = true
  for _, fname in ipairs(arg) do
    ok = check_file(fname, msgs) and ok
  end
  return ok
end


os.exit(main(arg))
