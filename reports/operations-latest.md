## 1. 작업 목표

기존 SupportSweepFast는 현재2독립 비교에서 적격 지붕 메서드 약26% 감소·지형 약6.6% 증가로 전역 gate를 통과하지 못했다. 구조물 FOLLOW에만 시도를 제한하는 default-OFF 국소 후보를 구현하고 기존8→64 기능 계약을 확인한다. 상태를 실제 지지 판정으로 대신하지 않고 정상 밀침·낙하·착지·추적·재등반을 보존한다.

## 2. 실제 수행

별도 nf_support_context PBO는 SolveContactsInternal wrapper를 추가했다. profile flag를 최초1회 읽으며 비활성·기존 global FAST/SHADOW·비적격 상태는 super로 위임한다. 적격 힌트는 GroundPlatform, FOLLOW, low/high, 실제 fixture 영역과 상태 영역 일치, active/dead combat 부재다. 그때만 기존 solver의 Fast 옵션 참에 해당하는 복사 메서드를 실행한다. 등록 컨테이너 중앙ray·전체 footprint box·실제 sphere sweep·법선·높이 가드와 모든 원래 joint/fallback 검사를 유지한다. 복사 메서드의 이름/옵션 두 토큰 변경을 되돌리면 원래 method와 정확히 일치한다. root/phase/native time·성공 횟수 쓰기나 false support 강제는 없다.

실제 native world/idle pose에서 synthetic state hints19조건을 시험했다. 지형·영역 불일치·platform false·MOTION/FALL/LAND/SETTLE/EGRESS/APPROACH·active/dead combat·기존 global owner·OFF는 원래 경로였다. 적격 지붕과 minimumLift0.3 거부 조건에서만 후보가 실행됐다. 그 뒤 기존 실제 플레이어 자극/상태 계약/마일스톤8명→64명 미션에 같은 후보를 로딩했다. 기존 Observe 주기에12열 사용 counter만 추가하고 원래 경로·실패 복구·시험 종료 조건은 바꾸지 않았다. client 준비 장면을 Computer Use로 확인하고 시작 gate를 열었다. 새 영상/native hook/Present 수집은 없다.

## 3. 측정 결과

19/19 가드 통과. 적격 지붕 비교 lift 차이는4.36401mm, 거부/나머지 조건의 반환/reason·root·pose·접촉점 계약 통과. 이는 일반 안전성이나 실제 authored climb19회가 아니다.

|시험|유효 최종 상태|낙하→착지→FOLLOW|후보 사용 개체|Fast 사용/생략 광선|원래 기능 gate|
|---|---|---|---|---|---|
|8명|8/8|8/8|8/8|11266/45064|True|
|64명|64/64|82/82|64/64|822487/3289948|True|

각 규모 독립1실행.8명 active 59.5537s, 전이136개;phase/generation/terminal 오류=0/0/0.64명 active 250.67s, 전이1226개;오류=0/0/0.64 server Mission OnUpdate p50/p95/p99(ms)={'p50': 50.0, 'p95': 75.0, 'p99': 90.0}. 이는 Present/CPU busy/GPU/애니메이션 표시 cadence가 아니다. 해당 수치·할당/GC·현재 wrapper의 지형 overhead·반복 OFF/ON 전체 비용은 미측정이다.20ms p95는 미달성 목표다.

64명 미해결 최종 결과: []

이력의 failure 라벨 전이 행수: {
  "CONTACT_SUPPORT_LOST": 79,
  "CLIMB_EXIT_VALIDATION": 4,
  "EGRESS_STALL": 1
}. 행수는 독립 실패 사건 수가 아니다. 최종 복구됐어도 CLIMB_EXIT_VALIDATION·EGRESS_STALL 이력을 삭제하거나 정상 밀침으로 재분류하지 않는다.

## 4. 원인 분석

확인: 실제 개체에서 후보를 사용한 흔적과 ray 감소가 있다. FastUsed당4ray 생략을 검증했다. 추가 bound sphere 수는 jointSweeps에 들어가지 않으므로 생략ray를 순수 총질의 절감이나CPU/프레임 절감으로 바꾸지 않는다. 표의 전체 gate와 미해결 상태를 그대로 판정한다. 과거 무조건 Fast64의63/64·복귀95/96은 보존되며 이번 결과로 해당 실패가 반증됐다고 하지 않는다. 초기 native 위상·접촉·날씨가 완전 일치하지 않아 완료시간 차이는 속도 개선 근거가 아니다.

가설: 지형에서 Fast 구조물 검사 overhead를 피해 적격 지붕의 효과를 유지할 수 있다. 현재 wrapper 자체의 분기 비용과 전체64개체 비용은 아직 측정하지 않았다. GroundPlatform/영역 라벨이 stale이어도 실제 physics guard를 건너뛸 수 없다. 그렇지만 보수적 lift 차이는 경로/접촉 이력에 영향을 줄 수 있어 기능 결과를 비용/화면 수용과 분리한다. 누적 counter의 현재 phase가 과거 모든 호출 phase를 증명하지는 않는다.

## 5. 검증 수준

PBO 빌드/packed bytes/서명:성공.19조건 상태 가드:제한 진단 통과.8/64:표의 기능 gate 결과 범위. 반복 회귀 완료·일반 맵/전투·시각 최종 수용·전체 성능 개선:미완료. client exit-1 및 소유 server 정리를 정상quit 인증으로 바꾸지 않아 normalQuitVerified=false다. 전체 골 active 유지.

## 6. 변경 사항

새 격리 PBO SHA256=1C879D1F7F0A82D9211A7832EC37732D37698342C4619B30FB4C11F98A523CC6. 자동 서명·DSCheckSignatures와 source/packed member 검증 성공,9개 모듈은 실제 실행 전 hash/서명을 검증했다. 기존8PBO·자산·운영서버·Steam·native 시계·이전 보고서·키는 변경하지 않았다. 개인키는 배포/공개물에 없다. 변경은 기존 research 내부 별도 후보 source/config·가드/실행 미션·분석/보고서다. 배포/최종 적용 후보로 승인한 상태가 아니다.

## 7. 원시 증거

sourceRunId:network-gameplay/support-context-8-64. candidate1의plan/정확한method 역변환·source·packed/build/sign, guards1의19행·실행 종료, eight/sixtyfour-support-context1의기존 마일스톤/gameplay/terminal/27열state contract·12열support-use·기존CE/pose-core·서버frame·서버/client 종료 및분석을 해시 목록에 연결한다. 공개는 집계·자체 후보 함수·가드 선정이며 원시 전체/영상/설치자산/로컬경로/프로세스/키는 제외한다.

## 8. 잔여 문제

Preserve actual 8/64 outcomes and failed history; do not infer full-frame benefit from skipped rays. Review any unresolved per-ID failure before promotion; only then repeated matched OFF/ON performance and actual visible quality. Pending-hit/death and broader combat interaction remain unvalidated.

먼저 미해결이 있으면 해당ID의 마지막 유효 상태·recovery generation·접촉·실패 원인부터 대조한다. 미해결이 없더라도 단일실행이라 반복 OFF/ON을 통한 인과/비용 확인이 필요하다. pending-hit/death와 피격 중 등반 같은 복합 상태는 이번19가드/이동 시험으로 수용하지 않는다. 실제 화면 부드러움과14클립·8파트 동기화는 별도 관찰이 필요하며 정지 screenshot으로 평가하지 않는다. 사용자의 이전 '애니메이션이 많이 나아졌다'는 주관적 피드백을 별도로 기록했고 측정/최종 수용으로 바꾸지 않았다. 실제 표시 ETW는 새 관리자 준비 후 구체적인 bounded capture로만 수행한다.

로컬 checkpoint SHA256: 14fce818fa9845bb743524b1fcbb872ef874f145c32cb21cc32cca307cd7a317
