/* Copyright (C) 1989-2024 Free Software Foundation, Inc.
     Written by James Clark (jjc@jclark.com)

This file is part of groff.

groff is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or
(at your option) any later version.

groff is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */


// diversions

#include "troff.h"
#include "dictionary.h"
#include "hvunits.h"
#include "stringclass.h"
#include "mtsm.h"
#include "env.h"
#include "request.h"
#include "node.h"
#include "token.h"
#include "div.h"
#include "reg.h"

#include "nonposix.h"

bool is_exit_underway = false;
bool is_eoi_macro_finished = false;
bool seen_last_page_ejector = false;
static bool began_page_in_eoi_macro = false;
int last_page_number = 0;	// if > 0, the number of the last page
				// specified with -o

static int last_post_line_extra_space = 0; // needed for \n(.a
static int nl_reg_contents = -1;
static int dl_reg_contents = 0;
static int dn_reg_contents = 0;
static bool honor_vertical_position_traps = true;
static vunits truncated_space;
static vunits needed_space;

diversion::diversion(symbol s)
: prev(0 /* nullptr */), nm(s), vertical_position(V0),
  high_water_mark(V0), is_in_no_space_mode(false),
  saved_seen_break(false), saved_seen_space(false),
  saved_seen_eol(false), saved_suppress_next_eol(false),
  marked_place(V0)
{
}

struct vertical_size {
  vunits pre_extra, post_extra, pre, post;
  vertical_size(vunits vs, vunits post_vs);
};

vertical_size::vertical_size(vunits vs, vunits post_vs)
: pre_extra(V0), post_extra(V0), pre(vs), post(post_vs)
{
}

void node::set_vertical_size(vertical_size *)
{
}

void extra_size_node::set_vertical_size(vertical_size *v)
{
  if (n < V0) {
    if (-n > v->pre_extra)
      v->pre_extra = -n;
  }
  else if (n > v->post_extra)
    v->post_extra = n;
}

void vertical_size_node::set_vertical_size(vertical_size *v)
{
  if (n < V0)
    v->pre = -n;
  else
    v->post = n;
}

top_level_diversion *topdiv;

diversion *curdiv;

void do_divert(bool appending, bool boxing)
{
  tok.skip();
  symbol nm = get_name();
  if (nm.is_null()) {
    if (curdiv->prev) {
      curenv->seen_break = curdiv->saved_seen_break;
      curenv->seen_space = curdiv->saved_seen_space;
      curenv->seen_eol = curdiv->saved_seen_eol;
      curenv->suppress_next_eol = curdiv->saved_suppress_next_eol;
      if (boxing) {
	curenv->line = curdiv->saved_line;
	curenv->width_total = curdiv->saved_width_total;
	curenv->space_total = curdiv->saved_space_total;
	curenv->saved_indent = curdiv->saved_saved_indent;
	curenv->target_text_length = curdiv->saved_target_text_length;
	curenv->prev_line_interrupted
	  = curdiv->saved_prev_line_interrupted;
      }
      diversion *temp = curdiv;
      curdiv = curdiv->prev;
      delete temp;
    }
    else
      warning(WARN_DI, "diversion stack underflow");
  }
  else {
    macro_diversion *md = new macro_diversion(nm, appending);
    md->prev = curdiv;
    curdiv = md;
    curdiv->saved_seen_break = curenv->seen_break;
    curdiv->saved_seen_space = curenv->seen_space;
    curdiv->saved_seen_eol = curenv->seen_eol;
    curdiv->saved_suppress_next_eol = curenv->suppress_next_eol;
    curenv->seen_break = 0;
    curenv->seen_space = 0;
    curenv->seen_eol = 0;
    if (boxing) {
      curdiv->saved_line = curenv->line;
      curdiv->saved_width_total = curenv->width_total;
      curdiv->saved_space_total = curenv->space_total;
      curdiv->saved_saved_indent = curenv->saved_indent;
      curdiv->saved_target_text_length = curenv->target_text_length;
      curdiv->saved_prev_line_interrupted
	= curenv->prev_line_interrupted;
      curenv->line = 0 /* nullptr */;
      curenv->start_line();
    }
  }
  skip_line();
}

void divert()
{
  do_divert(false /* appending */, false /* boxing */);
}

void divert_append()
{
  do_divert(true /* appending */, false /* boxing */);
}

void box()
{
  do_divert(false /* appending */, true /* boxing */);
}

void box_append()
{
  do_divert(true /* appending */, true /* appending */);
}

void diversion::need(vunits n)
{
  vunits d = distance_to_next_trap();
  if (d < n) {
    truncated_space = -d;
    needed_space = n;
    space(d, 1);
  }
}

macro_diversion::macro_diversion(symbol s, bool appending)
: diversion(s), max_width(H0), diversion_trap(0 /* nullptr */),
  diversion_trap_pos(0)
{
#if 0
  if (append) {
    /* We don't allow recursive appends, e.g.:

      .da a
      .a
      .di

      This causes an infinite loop in troff anyway.
      This is because the user could do

      .as a foo

      in the diversion, and this would mess things up royally,
      since there would be two things appending to the same
      macro_header.
      To make it work, we would have to copy the _contents_
      of the macro into which we were diverting; this doesn't
      strike me as worthwhile.
      However,

      .di a
      .a
      .a
      .di

       will work and will make 'a' contain two copies of what it
       contained before; in troff, 'a' would contain nothing. */
    request_or_macro *rm
      = static_cast<request_or_macro *>(request_dictionary.remove(s));
    if (!rm || (0 /* nullptr */ == (mac = rm->to_macro()))
      mac = new macro;
  }
  else
    mac = new macro;
#endif
  // We can now catch the situation described above by comparing
  // the length of the charlist in the macro_header with the length
  // stored in the macro. When we detect this, we copy the contents.
  mac = new macro(true /* is diversion */);
  if (appending) {
    request_or_macro *rm
      = static_cast<request_or_macro *>(request_dictionary.lookup(s));
    if (rm) {
      macro *m = rm->to_macro();
      if (m)
	*mac = *m;
    }
  }
}

macro_diversion::~macro_diversion()
{
  request_or_macro *rm
    = static_cast<request_or_macro *>(request_dictionary.lookup(nm));
  macro *m = rm ? rm->to_macro() : 0 /* nullptr */;
  if (m) {
    *m = *mac;
    delete mac;
  }
  else
    request_dictionary.define(nm, mac);
  mac = 0 /* nullptr */;
  dl_reg_contents = max_width.to_units();
  dn_reg_contents = vertical_position.to_units();
}

static int DIVERSION_LENGTH_MAX = INT_MAX;

vunits macro_diversion::distance_to_next_trap()
{
  vunits distance = 0;
  if (!diversion_trap.is_null()
      && (diversion_trap_pos > vertical_position))
    distance = diversion_trap_pos - vertical_position;
  else
    // Do the (saturating) arithmetic ourselves to avoid an error
    // diagnostic from constructor in number.cpp.
    distance = units(DIVERSION_LENGTH_MAX / vresolution);
  assert(distance >= 0);
  return distance;
}

const char *macro_diversion::get_next_trap_name()
{
  if (!diversion_trap.is_null()
      && (diversion_trap_pos > vertical_position))
    return diversion_trap.contents();
  else
    return "";
}

void macro_diversion::transparent_output(unsigned char c)
{
  mac->append(c);
}

void macro_diversion::transparent_output(node *n)
{
  mac->append(n);
}

void macro_diversion::output(node *nd, bool retain_size,
			     vunits vs, vunits post_vs, hunits width)
{
  is_in_no_space_mode = false;
  vertical_size v(vs, post_vs);
  while (nd != 0 /* nullptr */) {
    nd->set_vertical_size(&v);
    node *temp = nd;
    nd = nd->next;
    if (temp->interpret(mac))
      delete temp;
    else {
#if 1
      temp->freeze_space();
#endif
      mac->append(temp);
    }
  }
  last_post_line_extra_space = v.post_extra.to_units();
  if (!retain_size) {
    v.pre = vs;
    v.post = post_vs;
  }
  if (width > max_width)
    max_width = width;
  vunits x = v.pre + v.pre_extra + v.post + v.post_extra;
  int new_vpos = 0;
  int vpos = vertical_position.to_units();
  int lineht = x.to_units();
  bool overflow = false;
  if (ckd_add(&new_vpos, vpos, lineht))
    overflow = true;
  else if (new_vpos > DIVERSION_LENGTH_MAX)
    overflow = true;
  if (overflow)
    fatal("diversion overflow (vertical position: %1u,"
	  " next line height: %2u)", vpos, lineht);
  if (honor_vertical_position_traps
      && !diversion_trap.is_null()
      && (diversion_trap_pos > vertical_position)
      && (diversion_trap_pos <= vertical_position + x)) {
    vunits trunc = vertical_position + x - diversion_trap_pos;
    if (trunc > v.post)
      trunc = v.post;
    v.post -= trunc;
    x -= trunc;
    truncated_space = trunc;
    spring_trap(diversion_trap);
  }
  mac->append(new vertical_size_node(-v.pre));
  mac->append(new vertical_size_node(v.post));
  mac->append('\n');
  vertical_position += x;
  if (vertical_position - v.post > high_water_mark)
    high_water_mark = vertical_position - v.post;
}

void macro_diversion::space(vunits n, bool /* forcing */)
{
  if (honor_vertical_position_traps
      && !diversion_trap.is_null()
      && diversion_trap_pos > vertical_position
      && diversion_trap_pos <= vertical_position + n) {
    truncated_space = vertical_position + n - diversion_trap_pos;
    n = diversion_trap_pos - vertical_position;
    spring_trap(diversion_trap);
  }
  else if (n + vertical_position < V0)
    n = -vertical_position;
  mac->append(new diverted_space_node(n));
  vertical_position += n;
}

void macro_diversion::copy_file(const char *filename)
{
  mac->append(new diverted_copy_file_node(filename));
}

top_level_diversion::top_level_diversion()
: page_number(0), page_count(0), last_page_count(-1),
  page_length(units_per_inch*11),
  prev_page_offset(units_per_inch), page_offset(units_per_inch),
  page_trap_list(0 /* nullptr */), overriding_next_page_number(false),
  ejecting_page(false), before_first_page_status(1)
{
}

// find the next trap after pos

trap *top_level_diversion::find_next_trap(vunits *next_trap_pos)
{
  trap *next_trap = 0 /* nullptr */;
  for (trap *pt = page_trap_list; pt != 0 /* nullptr */; pt = pt->next)
    if (!pt->nm.is_null()) {
      if (pt->position >= V0) {
	if ((pt->position > vertical_position)
	    && (pt->position < page_length)
	    && ((0 /* nullptr */ == next_trap)
	        || pt->position < *next_trap_pos)) {
	  next_trap = pt;
	  *next_trap_pos = pt->position;
	}
      }
      else {
	vunits pos = pt->position;
	pos += page_length;
	if ((pos > 0)
	    && (pos > vertical_position)
	    && ((0 /* nullptr */ == next_trap)
		|| (pos < *next_trap_pos))) {
	  next_trap = pt;
	  *next_trap_pos = pos;
	}
      }
    }
  return next_trap;
}

vunits top_level_diversion::distance_to_next_trap()
{
  vunits d;
  if (!find_next_trap(&d))
    return page_length - vertical_position;
  else
    return d - vertical_position;
}

const char *top_level_diversion::get_next_trap_name()
{
  vunits next_trap_pos;
  trap *next_trap = find_next_trap(&next_trap_pos);
  if (0 /* nullptr */ == next_trap)
    return "";
  else
    return next_trap->nm.contents();
}

// This is used by more than just top-level diversions.
void top_level_diversion::output(node *nd, bool retain_size,
				 vunits vs, vunits post_vs,
				 hunits width)
{
  is_in_no_space_mode = false;
  vunits next_trap_pos;
  trap *next_trap = find_next_trap(&next_trap_pos);
  if ((before_first_page_status > 0) && begin_page())
    fatal("attempting diversion output before first page has started,"
	  " when a top-of-page trap is defined; invoke break or flush"
	  " request beforehand");
  vertical_size v(vs, post_vs);
  for (node *tem = nd; tem != 0 /* nullptr */; tem = tem->next)
    tem->set_vertical_size(&v);
  last_post_line_extra_space = v.post_extra.to_units();
  if (!retain_size) {
    v.pre = vs;
    v.post = post_vs;
  }
  vertical_position += v.pre;
  vertical_position += v.pre_extra;
  the_output->print_line(page_offset, vertical_position, nd,
			 v.pre + v.pre_extra, v.post_extra, width);
  vertical_position += v.post_extra;
  if (vertical_position > high_water_mark)
    high_water_mark = vertical_position;
  if (honor_vertical_position_traps && vertical_position >= page_length)
    begin_page();
  else if (honor_vertical_position_traps
	   && (next_trap != 0 /* nullptr */)
	   && (vertical_position >= next_trap_pos)) {
    nl_reg_contents = vertical_position.to_units();
    truncated_space = v.post;
    spring_trap(next_trap->nm);
  }
  else if (v.post > V0) {
    vertical_position += v.post;
    if (honor_vertical_position_traps
	&& (next_trap != 0 /* nullptr */)
	&& (vertical_position >= next_trap_pos)) {
      truncated_space = vertical_position - next_trap_pos;
      vertical_position = next_trap_pos;
      nl_reg_contents = vertical_position.to_units();
      spring_trap(next_trap->nm);
    }
    else if (honor_vertical_position_traps
	     && (vertical_position >= page_length))
      begin_page();
    else
      nl_reg_contents = vertical_position.to_units();
  }
  else
    nl_reg_contents = vertical_position.to_units();
}

// The next two member functions implement the internals of `.output`
// and `\!`.

void top_level_diversion::transparent_output(unsigned char c)
{
  if ((before_first_page_status > 0) && begin_page())
    fatal("attempting transparent output from top-level diversion"
	  " before first page has started, when a top-of-page trap is"
	  " defined; invoke break or flush request beforehand");
  const char *s = asciify(c);
  while (*s)
    the_output->transparent_char(*s++);
}

void top_level_diversion::transparent_output(node * /*n*/)
{
  // TODO: When Savannah #63074 is fixed, the user will have a way to
  // avoid this error.
  error("cannot write a node to device-independent output");
}

// Implement the internals of `.cf`.
void top_level_diversion::copy_file(const char *filename)
{
  if ((before_first_page_status > 0) && begin_page())
    fatal("attempting transparent copy of file to top-level diversion"
	  " before first page has started, when a top-of-page trap is"
	  " defined; invoke break or flush request beforehand");
  the_output->copy_file(page_offset, vertical_position, filename);
}

void top_level_diversion::space(vunits n, bool forcing)
{
  if (is_in_no_space_mode) {
    if (!forcing)
      return;
    else
      is_in_no_space_mode = false;
  }
  if (before_first_page_status > 0) {
    begin_page(n);
    return;
  }
  vunits next_trap_pos;
  trap *next_trap = find_next_trap(&next_trap_pos);
  vunits y = vertical_position + n;
  if (curenv->get_vertical_spacing().to_units())
    curenv->seen_space += n.to_units()
			  / curenv->get_vertical_spacing().to_units();
  if (honor_vertical_position_traps
      && (next_trap != 0 /* nullptr */)
      && (y >= next_trap_pos)) {
    vertical_position = next_trap_pos;
    nl_reg_contents = vertical_position.to_units();
    truncated_space = y - vertical_position;
    spring_trap(next_trap->nm);
  }
  else if (y < V0) {
    vertical_position = V0;
    nl_reg_contents = vertical_position.to_units();
  }
  else if (honor_vertical_position_traps
	   && (y >= page_length && n >= V0))
    begin_page(y - page_length);
  else {
    vertical_position = y;
    nl_reg_contents = vertical_position.to_units();
  }
}

trap::trap(symbol s, vunits n, trap *p)
: next(p), position(n), nm(s)
{
}

void top_level_diversion::add_trap(symbol nam, vunits pos)
{
  trap *first_free_slot = 0 /* nullptr*/;
  trap **p;
  for (p = &page_trap_list; *p; p = &(*p)->next) {
    if ((*p)->nm.is_null()) {
      if (0 /* nullptr*/ == first_free_slot)
	first_free_slot = *p;
    }
    else if ((*p)->position == pos) {
      (*p)->nm = nam;
      return;
    }
  }
  if (first_free_slot) {
    first_free_slot->nm = nam;
    first_free_slot->position = pos;
  }
  else
    *p = new trap(nam, pos, 0 /* nullptr*/);
}

void top_level_diversion::remove_trap(symbol nam)
{
  for (trap *p = page_trap_list; p; p = p->next)
    if (p->nm == nam) {
      p->nm = NULL_SYMBOL;
      return;
    }
}

void top_level_diversion::remove_trap_at(vunits pos)
{
  for (trap *p = page_trap_list; p; p = p->next)
    if (p->position == pos) {
      p->nm = NULL_SYMBOL;
      return;
    }
}

void top_level_diversion::change_trap(symbol nam, vunits pos)
{
  for (trap *p = page_trap_list; p; p = p->next)
    if (p->nm == nam) {
      p->position = pos;
      return;
    }
  warning(WARN_MAC, "cannot move unplanted trap macro '%1'",
	  nam.contents());
}

void top_level_diversion::print_traps()
{
  for (trap *p = page_trap_list; p; p = p->next)
    if (p->nm.is_null())
      fprintf(stderr, "  empty\n");
    else
      fprintf(stderr, "%s\t%d\n", p->nm.contents(),
	      p->position.to_units());
  fflush(stderr);
}

void end_diversions()
{
  while (curdiv != topdiv) {
    error("automatically ending diversion '%1' on exit",
	    curdiv->nm.contents());
    diversion *tem = curdiv;
    curdiv = curdiv->prev;
    delete tem;
  }
}

void cleanup_and_exit(int exit_code)
{
  if (the_output != 0 /* nullptr */) {
    the_output->trailer(topdiv->get_page_length());
    // If we're already dying, don't call the_output's destructor.  See
    // node.cpp:real_output_file::~real_output_file().
    if (!the_output->is_dying)
      delete the_output;
  }
  FLUSH_INPUT_PIPE(STDIN_FILENO);
  exit(exit_code);
}

// Returns `true` if beginning the page sprung a top-of-page trap.
// The optional parameter is for the .trunc register.
bool top_level_diversion::begin_page(vunits n)
{
  if (is_exit_underway) {
    if (page_count == last_page_count
	? curenv->is_empty()
	: (is_eoi_macro_finished && (seen_last_page_ejector
				      || began_page_in_eoi_macro)))
      cleanup_and_exit(EXIT_SUCCESS);
    if (!is_eoi_macro_finished)
      began_page_in_eoi_macro = true;
  }
  if (last_page_number > 0 && page_number == last_page_number)
    cleanup_and_exit(EXIT_SUCCESS);
  if (0 /* nullptr */ == the_output)
    init_output();
  ++page_count;
  if (overriding_next_page_number) {
    page_number = next_page_number;
    overriding_next_page_number = false;
  }
  else if (before_first_page_status == 1)
    page_number = 1;
  else
    page_number++;
  // spring the top of page trap if there is one
  vunits next_trap_pos;
  vertical_position = -vresolution;
  trap *next_trap = find_next_trap(&next_trap_pos);
  vertical_position = V0;
  high_water_mark = V0;
  ejecting_page = false;
  // If before_first_page_status was 2, then the top of page transition
  // was undone using ".nr nl 0-1" or similar.  See nl_reg::set_value.
  if (before_first_page_status != 2)
    the_output->begin_page(page_number, page_length);
  before_first_page_status = 0;
  nl_reg_contents = vertical_position.to_units();
  if ((honor_vertical_position_traps && (next_trap != 0 /* nullptr */))
      && (next_trap_pos == V0)) {
    truncated_space = n;
    spring_trap(next_trap->nm);
    return true;
  }
  else
    return false;
}

void continue_page_eject()
{
  if (topdiv->get_ejecting()) {
    if (curdiv != topdiv)
      error("cannot continue page ejection while diverting output");
    else if (!honor_vertical_position_traps)
      error("cannot continue page ejection while vertical position"
	    " traps disabled");
    else {
      push_page_ejector();
      topdiv->space(topdiv->get_page_length(), true /* forcing */);
    }
  }
}

void top_level_diversion::set_next_page_number(int n)
{
  next_page_number = n;
  overriding_next_page_number = true;
}

int top_level_diversion::get_next_page_number()
{
  return overriding_next_page_number ? next_page_number
				     : (page_number + 1);
}

void top_level_diversion::set_page_length(vunits n)
{
  page_length = n;
}

diversion::~diversion()
{
}

void page_offset()
{
  hunits n;
  // The troff manual says that the default scaling indicator is v,
  // but it is in fact m: v wouldn't make sense for a horizontally
  // oriented request.
  if (!has_arg() || !get_hunits(&n, 'm', topdiv->page_offset))
    n = topdiv->prev_page_offset;
  topdiv->prev_page_offset = topdiv->page_offset;
  topdiv->page_offset = n;
  topdiv->modified_tag.incl(MTSM_PO);
  skip_line();
}

void page_length()
{
  vunits temp;
  if (has_arg() && get_vunits(&temp, 'v', topdiv->get_page_length())) {
    if (temp < vresolution) {
      warning(WARN_RANGE, "setting computed page length %1u to device"
			  " vertical motion quantum",
			  temp.to_units());
      temp = vresolution;
    }
    topdiv->set_page_length(temp);
  }
  else
    topdiv->set_page_length(11 * units_per_inch);
  skip_line();
}

void when_request()
{
  vunits n;
  if (get_vunits(&n, 'v')) {
    symbol s = get_name();
    if (s.is_null())
      topdiv->remove_trap_at(n);
    else
      topdiv->add_trap(s, n);
  }
  skip_line();
}

void begin_page()
{
  bool got_arg = false;
  int n = 0;
  if (has_arg() && get_integer(&n, topdiv->get_page_number()))
    got_arg = true;
  while (!tok.is_newline() && !tok.is_eof())
    tok.next();
  if (curdiv == topdiv) {
    if (topdiv->before_first_page_status > 0) {
      if (!want_break) {
	if (got_arg)
	  topdiv->set_next_page_number(n);
	if (got_arg || !topdiv->is_in_no_space_mode)
	  topdiv->begin_page();
      }
      else if (topdiv->is_in_no_space_mode && !got_arg)
	topdiv->begin_page();
      else {
	/* Given this

         .wh 0 x
	 .de x
	 .tm \\n%
	 ..
	 .bp 3

	 troff prints

	 1
	 3

	 This code makes groff do the same. */

	push_page_ejector();
	topdiv->begin_page();
	if (got_arg)
	  topdiv->set_next_page_number(n);
	topdiv->set_ejecting();
      }
    }
    else {
      push_page_ejector();
      if (want_break)
	curenv->do_break();
      if (got_arg)
	topdiv->set_next_page_number(n);
      if (!(topdiv->is_in_no_space_mode && !got_arg))
	topdiv->set_ejecting();
    }
  }
  tok.next();
}

void no_space()
{
  curdiv->is_in_no_space_mode = true;
  skip_line();
}

void restore_spacing()
{
  curdiv->is_in_no_space_mode = false;
  skip_line();
}

/* It is necessary to generate a break before reading the argument,
because otherwise arguments using | will be wrong.  But if we just
generate a break as usual, then the line forced out may spring a trap
and thus push a macro onto the input stack before we have had a chance
to read the argument to the sp request.  We resolve this dilemma by
setting, before generating the break, a flag which will postpone the
actual pushing of the macro associated with the trap sprung by the
outputting of the line forced out by the break till after we have read
the argument to the request.  If the break did cause a trap to be
sprung, then we don't actually do the space. */

void space_request()
{
  postpone_traps();
  if (want_break)
    curenv->do_break();
  vunits n;
  if (!has_arg() || !get_vunits(&n, 'v'))
    n = curenv->get_vertical_spacing();
  while (!tok.is_newline() && !tok.is_eof())
    tok.next();
  if (!unpostpone_traps() && !curdiv->is_in_no_space_mode)
    curdiv->space(n);
  else
    // The line might have had line spacing that was truncated.
    truncated_space += n;

  tok.next();
}

void blank_line()
{
  curenv->do_break();
  if (!was_trap_sprung && !curdiv->is_in_no_space_mode)
    curdiv->space(curenv->get_vertical_spacing());
  else
    truncated_space += curenv->get_vertical_spacing();
}

/* need_space might spring a trap and so we must be careful that the
BEGIN_TRAP token is not skipped over. */

void need_space()
{
  vunits n;
  if (!has_arg() || !get_vunits(&n, 'v'))
    n = curenv->get_vertical_spacing();
  while (!tok.is_newline() && !tok.is_eof())
    tok.next();
  curdiv->need(n);
  tok.next();
}

void page_number()
{
  int n = 0;
  // the ps4html register is set if we are using -Tps
  // to generate images for html
  // XXX: Yuck!  Get rid of this; macro packages already test the
  // register before invoking .pn.
  reg *r = static_cast<reg *>(register_dictionary.lookup("ps4html"));
  if (0 /* nullptr */ == r)
    if (has_arg() && get_integer(&n, topdiv->get_page_number()))
      topdiv->set_next_page_number(n);
  skip_line();
}

vunits saved_space;

void save_vertical_space()
{
  vunits x;
  if (!has_arg() || !get_vunits(&x, 'v'))
    x = curenv->get_vertical_spacing();
  if (curdiv->distance_to_next_trap() > x)
    curdiv->space(x, true /* forcing */);
  else
    saved_space = x;
  skip_line();
}

void output_saved_vertical_space()
{
  while (!tok.is_newline() && !tok.is_eof())
    tok.next();
  if (saved_space > V0)
    curdiv->space(saved_space, true /* forcing */);
  saved_space = V0;
  tok.next();
}

static void flush_request()
{
  while (!tok.is_newline() && !tok.is_eof())
    tok.next();
  if (want_break)
    curenv->do_break();
  if (the_output != 0 /* nullptr */)
    the_output->flush();
  tok.next();
}

void macro_diversion::set_diversion_trap(symbol s, vunits n)
{
  diversion_trap = s;
  diversion_trap_pos = n;
}

void macro_diversion::clear_diversion_trap()
{
  diversion_trap = NULL_SYMBOL;
}

void top_level_diversion::set_diversion_trap(symbol, vunits)
{
  error("cannot set diversion trap when not diverting output");
}

void top_level_diversion::clear_diversion_trap()
{
  error("cannot clear diversion trap when not diverting output");
}

void diversion_trap()
{
  vunits n;
  if (has_arg() && get_vunits(&n, 'v')) {
    symbol s = get_name();
    if (!s.is_null())
      curdiv->set_diversion_trap(s, n);
    else
      curdiv->clear_diversion_trap();
  }
  else
    curdiv->clear_diversion_trap();
  skip_line();
}

void change_trap()
{
  symbol s = get_name(true /* required */);
  if (!s.is_null()) {
    vunits x;
    if (has_arg() && get_vunits(&x, 'v'))
      topdiv->change_trap(s, x);
    else
      topdiv->remove_trap(s);
  }
  skip_line();
}

void print_traps()
{
  topdiv->print_traps();
  skip_line();
}

void mark()
{
  symbol s = get_name();
  if (s.is_null())
    curdiv->marked_place = curdiv->get_vertical_position();
  else if (curdiv == topdiv)
    set_register(s, nl_reg_contents);
  else
    set_register(s, curdiv->get_vertical_position().to_units());
  skip_line();
}

// This is truly bizarre.  It is documented in the SQ manual.

void return_request()
{
  vunits dist = curdiv->marked_place - curdiv->get_vertical_position();
  if (has_arg()) {
    if (tok.ch() == '-') {
      tok.next();
      vunits x;
      if (get_vunits(&x, 'v'))
	dist = -x;
    }
    else {
      vunits x;
      if (get_vunits(&x, 'v'))
	dist = x >= V0 ? x - curdiv->get_vertical_position() : V0;
    }
  }
  if (dist < V0)
    curdiv->space(dist);
  skip_line();
}

void vertical_position_traps()
{
  int n = 0;
  if (has_arg() && get_integer(&n))
    honor_vertical_position_traps = (n > 0);
  else
    honor_vertical_position_traps = true;
  skip_line();
}

class page_offset_reg : public reg {
public:
  bool get_value(units *);
  const char *get_string();
};

bool page_offset_reg::get_value(units *res)
{
  *res = topdiv->get_page_offset().to_units();
  return true;
}

const char *page_offset_reg::get_string()
{
  return i_to_a(topdiv->get_page_offset().to_units());
}

class page_length_reg : public reg {
public:
  bool get_value(units *);
  const char *get_string();
};

bool page_length_reg::get_value(units *res)
{
  *res = topdiv->get_page_length().to_units();
  return true;
}

const char *page_length_reg::get_string()
{
  return i_to_a(topdiv->get_page_length().to_units());
}

class vertical_position_reg : public reg {
public:
  bool get_value(units *);
  const char *get_string();
};

bool vertical_position_reg::get_value(units *res)
{
  if ((curdiv == topdiv) && (topdiv->before_first_page_status > 0))
    *res = -1;
  else
    *res = curdiv->get_vertical_position().to_units();
  return true;
}

const char *vertical_position_reg::get_string()
{
  if ((curdiv == topdiv) && (topdiv->before_first_page_status > 0))
    return "-1";
  else
    return i_to_a(curdiv->get_vertical_position().to_units());
}

class high_water_mark_reg : public reg {
public:
  bool get_value(units *);
  const char *get_string();
};

bool high_water_mark_reg::get_value(units *res)
{
  *res = curdiv->get_high_water_mark().to_units();
  return true;
}

const char *high_water_mark_reg::get_string()
{
  return i_to_a(curdiv->get_high_water_mark().to_units());
}

class distance_to_next_trap_reg : public reg {
public:
  bool get_value(units *);
  const char *get_string();
};

bool distance_to_next_trap_reg::get_value(units *res)
{
  *res = curdiv->distance_to_next_trap().to_units();
  return true;
}

const char *distance_to_next_trap_reg::get_string()
{
  return i_to_a(curdiv->distance_to_next_trap().to_units());
}

class diversion_name_reg : public reg {
public:
  const char *get_string();
};

const char *diversion_name_reg::get_string()
{
  return curdiv->get_diversion_name();
}

class next_trap_name_reg : public reg {
public:
  const char *get_string();
};

const char *next_trap_name_reg::get_string()
{
  return curdiv->get_next_trap_name();
}

class page_number_reg : public general_reg {
public:
  page_number_reg();
  bool get_value(units *);
  void set_value(units);
};

page_number_reg::page_number_reg()
{
}

void page_number_reg::set_value(units n)
{
  topdiv->set_page_number(n);
}

bool page_number_reg::get_value(units *res)
{
  *res = topdiv->get_page_number();
  return true;
}

class next_page_number_reg : public reg {
public:
  const char *get_string();
};

const char *next_page_number_reg::get_string()
{
  return i_to_a(topdiv->get_next_page_number());
}

class page_ejecting_reg : public reg {
public:
  const char *get_string();
};

const char *page_ejecting_reg::get_string()
{
  return i_to_a(topdiv->get_ejecting());
}

class constant_vunits_reg : public reg {
  vunits *p;
public:
  constant_vunits_reg(vunits *);
  const char *get_string();
};

constant_vunits_reg::constant_vunits_reg(vunits *q) : p(q)
{
}

const char *constant_vunits_reg::get_string()
{
  return i_to_a(p->to_units());
}

class nl_reg : public variable_reg {
public:
  nl_reg();
  void set_value(units);
};

nl_reg::nl_reg() : variable_reg(&nl_reg_contents)
{
}

void nl_reg::set_value(units n)
{
  variable_reg::set_value(n);
  // Setting nl to a negative value when the vertical position in
  // the top-level diversion is 0 undoes the top of page transition,
  // so that the header macro will be called as if the top of page
  // transition hasn't happened.  This is used by Larry Wall's
  // wrapman program.  Setting before_first_page_status to 2 rather than
  // 1, tells top_level_diversion::begin_page not to call
  // output_file::begin_page again.
  if (n < 0 && topdiv->get_vertical_position() == V0)
    topdiv->before_first_page_status = 2;
}

class no_space_mode_reg : public reg {
public:
  bool get_value(units *);
  const char *get_string();
};

bool no_space_mode_reg::get_value(units *val)
{
  *val = curdiv->is_in_no_space_mode;
  return true;
}

const char *no_space_mode_reg::get_string()
{
  return curdiv->is_in_no_space_mode ? "1" : "0";
}

void init_div_requests()
{
  init_request("box", box);
  init_request("boxa", box_append);
  init_request("bp", begin_page);
  init_request("ch", change_trap);
  init_request("da", divert_append);
  init_request("di", divert);
  init_request("dt", diversion_trap);
  init_request("fl", flush_request);
  init_request("mk", mark);
  init_request("ne", need_space);
  init_request("ns", no_space);
  init_request("os", output_saved_vertical_space);
  init_request("pl", page_length);
  init_request("pn", page_number);
  init_request("po", page_offset);
  init_request("ptr", print_traps);
  init_request("rs", restore_spacing);
  init_request("rt", return_request);
  init_request("sp", space_request);
  init_request("sv", save_vertical_space);
  init_request("vpt", vertical_position_traps);
  init_request("wh", when_request);
  register_dictionary.define(".a",
      new readonly_register(&last_post_line_extra_space));
  register_dictionary.define(".d", new vertical_position_reg);
  register_dictionary.define(".h", new high_water_mark_reg);
  register_dictionary.define(".ne",
      new constant_vunits_reg(&needed_space));
  register_dictionary.define(".ns", new no_space_mode_reg);
  register_dictionary.define(".o", new page_offset_reg);
  register_dictionary.define(".p", new page_length_reg);
  register_dictionary.define(".pe", new page_ejecting_reg);
  register_dictionary.define(".pn", new next_page_number_reg);
  register_dictionary.define(".t", new distance_to_next_trap_reg);
  register_dictionary.define(".trap", new next_trap_name_reg);
  register_dictionary.define(".trunc",
      new constant_vunits_reg(&truncated_space));
  register_dictionary.define(".vpt",
      new readonly_boolean_register(&honor_vertical_position_traps));
  register_dictionary.define(".z", new diversion_name_reg);
  register_dictionary.define("dl", new variable_reg(&dl_reg_contents));
  register_dictionary.define("dn", new variable_reg(&dn_reg_contents));
  register_dictionary.define("nl", new nl_reg);
  register_dictionary.define("%", new page_number_reg);
}

// Local Variables:
// fill-column: 72
// mode: C++
// End:
// vim: set cindent noexpandtab shiftwidth=2 textwidth=72:
