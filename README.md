# 오토마타 (Automata)

> Originally created in 2009.  
> 최초 작성: 2009년

---

## 한국어

### 라이선스 및 사용 조건

본 프로젝트(오토마타, Automata)는 2009년에 최초 작성되었습니다.

본 프로젝트는 학습, 연구, 개인적인 실험 목적으로 자유롭게 사용할 수 있습니다.

다만 아래 사항을 반드시 준수해야 합니다.

- 본 프로젝트 및 소스코드를 무단으로 재배포하거나 저작권을 침해하는 행위를 금지합니다.
- 원저작자의 고지, 라이선스, 저작권 표기를 제거하거나 변경할 수 없습니다.
- 본 프로젝트를 이용한 상업적 이용(유료 서비스, 판매, SaaS 제공, 기업 내부 상용 활용 등)은 사전 서면 허가 없이 금지됩니다.
- 본 프로젝트의 일부 또는 전체를 무단으로 복제, 판매, 재라이선스하는 행위를 금지합니다.
- 본 프로젝트를 활용하여 발생하는 문제나 손해에 대해 저작자는 책임을 지지 않습니다.

상업적 이용 또는 별도 라이선스 문의는 저작자에게 연락해주시기 바랍니다.

---

## English

### License & Usage Terms

This project (Automata) was originally created in 2009.

This project may be freely used for educational, research, and personal experimental purposes.

However, the following conditions must be strictly followed:

- Unauthorized redistribution or copyright infringement of this project or its source code is prohibited.
- Removal or modification of copyright notices, license information, or author attribution is not allowed.
- Commercial use of this project (including paid services, SaaS, resale, enterprise/internal commercial usage, etc.) is prohibited without prior written permission from the author.
- Copying, relicensing, or selling all or part of this project without authorization is prohibited.
- The author is not responsible for any damages, issues, or liabilities arising from the use of this project.

For commercial licensing or other inquiries, please contact the author.

---

Copyright © 2009-present Automata Project. All rights reserved.


---
# C++ 코드 분석: 정규표현식 → NFA → DFA 변환기

<img width="1472" height="320" alt="image" src="https://github.com/user-attachments/assets/85142269-42da-4956-8b9e-27e7999da659" />

---

## 1. 자료구조

```cpp
typedef struct STATE {
    char state_name;      // 상태 이름 (A, B, C ... ASCII 순)
    bool final_state;     // 최종 상태 여부
    STATE* a_state;       // 'a' 입력 시 전이
    STATE* b_state;       // 'b' 입력 시 전이
    STATE* Ramda_state1;  // λ(람다, epsilon) 전이 1
    STATE* Ramda_state2;  // λ(람다, epsilon) 전이 2
} STATE;
```

각 NFA 상태를 연결 리스트처럼 포인터로 연결합니다. 알파벳은 `a`, `b` 두 가지만 지원합니다.

---

## 2. 주요 함수 분석

### `postfix()` — 중위식 → 후위식 변환
연산자 우선순위 기반 스택을 이용한 표준 **Shunting-yard 알고리즘**입니다.

| 연산자 | isp (스택 내) | icp (입력) |
|--------|-------------|-----------|
| `\|` (OR) | 1 | 1 |
| `.` (AND) | 2 | 2 |
| `(` | 0 | 3 |

### `REtoNFA()` — 후위식 → NFA (Thompson 구성법)
후위식 스택을 순회하며 각 연산자에 대해 NFA 조각을 생성하고 합칩니다.

- `a`, `b` → 단순 2-상태 NFA 생성
- `.` (연접) → 두 NFA를 λ-전이로 직렬 연결
- `|` (OR) → 새 시작/종료 상태를 만들어 두 NFA를 λ-전이로 병렬 연결
- `*` (Kleene) → 새 시작/종료 상태 + 루프 λ-전이 추가

### `NFA_Table()` / `NFA_Print()` — NFA 테이블 생성 및 출력
NFA 구조를 2차원 `char` 배열로 변환합니다. 각 행: `[상태명, a전이, b전이, λ1, λ2, S/F 표시]`

### `NFAtoDFA()` — NFA → DFA (λ-전이 병합)
λ-전이(`Ramda_state`)로 연결된 상태들을 하나로 **병합**하는 방식입니다. 표준 부분집합 구성법(subset construction)과는 다소 다른, 단순화된 접근입니다.

### `accept()` — 문자열 수락 판정
DFA 테이블을 따라가며 입력 문자열 처리 후 최종 상태 도달 여부를 반환합니다.

---

## 3. 한계 및 주의점

몇 가지 구조적 제한이 있습니다.

**알파벳 제한** — `a`와 `b`만 처리 가능하며 다른 문자는 지원하지 않습니다.

**NFA 순회 방식의 취약성** — `NFA_Table()`과 `NFA_Print()`에서 상태를 ASCII 순으로 탐색하는 로직이 복잡한 NFA 구조에서 누락될 수 있습니다.

**NFAtoDFA의 단순화** — 진정한 부분집합 구성법이 아닌 λ-전이 병합 방식이라, 일부 복잡한 정규표현식에서 정확하지 않을 수 있습니다.

**메모리 해제 없음** — `new`로 할당한 STATE 객체들을 `delete`하지 않아 메모리 누수가 발생합니다.

**전역 변수 `SIZE_TABLE`** — `accept()`에서 사용하는 테이블 크기를 전역으로 관리하여 구조적으로 취약합니다.

