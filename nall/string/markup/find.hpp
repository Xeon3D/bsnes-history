#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {
namespace Markup {

auto ManagedNode::_evaluate(string query) const -> bool {
  if(!query) return true;

  for(auto& rule : query.replace(" ", "").split(",")) {
    enum class Comparator : unsigned { ID, EQ, NE, LT, LE, GT, GE };
    auto comparator = Comparator::ID;
         if(rule.match("*!=*")) comparator = Comparator::NE;
    else if(rule.match("*<=*")) comparator = Comparator::LE;
    else if(rule.match("*>=*")) comparator = Comparator::GE;
    else if(rule.match ("*=*")) comparator = Comparator::EQ;
    else if(rule.match ("*<*")) comparator = Comparator::LT;
    else if(rule.match ("*>*")) comparator = Comparator::GT;

    if(comparator == Comparator::ID) {
      if(_find(rule).size()) continue;
      return false;
    }

    lstring side;
    switch(comparator) {
    case Comparator::EQ: side = rule.split<1> ("="); break;
    case Comparator::NE: side = rule.split<1>("!="); break;
    case Comparator::LT: side = rule.split<1> ("<"); break;
    case Comparator::LE: side = rule.split<1>("<="); break;
    case Comparator::GT: side = rule.split<1> (">"); break;
    case Comparator::GE: side = rule.split<1>(">="); break;
    }

    string data = string{_value}.strip();
    if(side(0).empty() == false) {
      auto result = _find(side(0));
      if(result.size() == 0) return false;
      data = result[0].value();
    }

    switch(comparator) {
    case Comparator::EQ: if(data.match(side(1)) ==  true)      continue; break;
    case Comparator::NE: if(data.match(side(1)) == false)      continue; break;
    case Comparator::LT: if(numeral(data)  < numeral(side(1))) continue; break;
    case Comparator::LE: if(numeral(data) <= numeral(side(1))) continue; break;
    case Comparator::GT: if(numeral(data)  > numeral(side(1))) continue; break;
    case Comparator::GE: if(numeral(data) >= numeral(side(1))) continue; break;
    }

    return false;
  }

  return true;
}

auto ManagedNode::_find(const string& query) const -> vector<Node> {
  vector<Node> result;

  lstring path = query.split("/");
  string name = path.take(0), rule;
  unsigned lo = 0u, hi = ~0u;

  if(name.match("*[*]")) {
    auto p = name.rtrim("]").split<1>("[");
    name = p(0);
    if(p(1).find("-")) {
      p = p(1).split<1>("-");
      lo = p(0).empty() ?  0u : numeral(p(0));
      hi = p(1).empty() ? ~0u : numeral(p(1));
    } else {
      lo = hi = numeral(p(1));
    }
  }

  if(name.match("*(*)")) {
    auto p = name.rtrim(")").split<1>("(");
    name = p(0);
    rule = p(1);
  }

  unsigned position = 0;
  for(auto& node : _children) {
    if(!node->_name.match(name)) continue;
    if(!node->_evaluate(rule)) continue;

    bool inrange = position >= lo && position <= hi;
    position++;
    if(!inrange) continue;

    if(path.size() == 0) {
      result.append(node);
    } else for(auto& item : node->_find(path.merge("/"))) {
      result.append(item);
    }
  }

  return result;
}

auto ManagedNode::_lookup(const string& path) const -> Node {
  if(auto position = path.find("/")) {
    auto name = path.slice(0, *position);
    for(auto& node : _children) {
      if(name == node->_name) {
        return node->_lookup(path.slice(*position + 1));
      }
    }
  } else for(auto& node : _children) {
    if(path == node->_name) return node;
  }
  return {};
}

auto ManagedNode::_create(const string& path) -> Node {
  if(auto position = path.find("/")) {
    auto name = path.slice(0, *position);
    for(auto& node : _children) {
      if(name == node->_name) {
        return node->_create(path.slice(*position + 1));
      }
    }
    _children.append(new ManagedNode(name));
    return _children.last()->_create(path.slice(*position + 1));
  }
  for(auto& node : _children) {
    if(path == node->_name) return node;
  }
  _children.append(new ManagedNode(path));
  return _children.last();
}

}
}

#endif
