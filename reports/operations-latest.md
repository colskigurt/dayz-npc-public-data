## 1. 작업 목표

지속적인 등반 종료 접촉에서 기존 solver가 요구한 약3.815mm의 표시 높이 차이를 불변 조건이 거부하던 문제를 국소적으로 수정한다. 정상 밀침·낙하·착지·추적·재등반과 기존 완료 함수를 보존한다. 시각 부드러움과20ms p95는 별도 목표다.

## 2. 실제 수행

새 default-OFF 후보는 기존 native 종료/request/time/endpoint·실제 현재 exit 거리·LandingSpace·양발 비침투·현재 player/core·4관절 full swept 검사를 모두 유지한다. solver가 수용한 checkedLift가 절대0~0.25m, 변화-0.0001~+0.01m 범위일 때만 대응 contacts와 함께 적용한다. 작은 음수 허용은 실수 오차 여유이며1cm는 실험 상한으로 일반 안전성 입증값이 아니다. root·native clock·curve flag·등반 횟수 직접 변경은 없다. 기존 SETTLE/EGRESS/CompleteEgress가 완료를 집계한다.4개의 관절/radius/contact 구성도 요구한다.

합성 종료 메타데이터/frozen pose와 실제 fixture·살아있는 플레이어로50개 가드를 실행했다. 작은 보정 수용·큰 보정 거부·발 침투·플레이어 현재 겹침·요청/시간/상태·throttle·골격 구성·root/counter/Active·실패 복원 및 적용된 lift/contact 일치를 검사했다. 합성 조건을 실제 authored 동작으로 간주하지 않는다.

그 뒤 동일 새PBO·동일 mission bytes에서 실제 authored 등반의 벽 정면 접촉을 ON→OFF 순서로 비교했다. 제어한 것은 시험 플레이어 배치이며 NPC root/time/end metadata 주입은 없다. 초기 위상·날씨·gate는 고정하지 않아 성능 A/B 또는 완전한 pose 일치 대조가 아니다. 이어 원래8명→64명 network 하네스를 그대로 실행했다. CUA로 장면·전경 확인 후 시작했고 native hook/영상은 추가하지 않았다.

## 3. 측정 결과

50/50 가드 통과, 양성4/음성46. 작은 보정 사례는 실제6.13281mm 적용과 대응 접촉점 일치를 확인했다. 현재 플레이어 겹침은 CURRENT_CORE에서 거부했다. 모든 root와 완료 횟수는 직접 변경0이었다.

실제 ON: checks/completions/rejections=1/1/0, hold=0s, 최종 FOLLOW·climbs2·failure=없음. 실제 OFF: 0/0/0, hold=2.05s, 최종 FOLLOW·climbs1·failure=SUPPORTED_CONTACT_REPLAN. 단일 기능 대비 판정=True.

ON 적용 journal: [
  {
    "seconds": "14.34",
    "id": "0",
    "oldLift": "0.157715",
    "checkedLift": "0.16156",
    "appliedLift": "0.16156",
    "deltaLift": "0.00384521",
    "rootBefore": "<4512.328613, 342.167847, 10237.577148>",
    "rootAfter": "<4512.328613, 342.167847, 10237.577148>",
    "pointCount": "4"
  }
]

|시험|유효 최종 상태|low/high/down/returned 누적|낙하→착지→FOLLOW|후보 검사/완료/거부|원래 전체 gate|
|---|---|---|---|---|---|
|8명|8/8|{'low': 8, 'high': 8, 'down': 8, 'returned': 8}|9/9|0/0/0|True|
|64명|64/64|{'low': 64, 'high': 64, 'down': 64, 'returned': 64}|97/97|0/0/0|True|

각 조건 독립1회.8명 전이144개, 정합 오류phase/generation/terminal=0/0/0.64명 전이1359개, 정합 오류=0/0/0.64 server Mission dt p50/p95/p99(ms)={'p50': 49.0, 'p95': 72.0, 'p99': 81.0}. 이는 서버 OnUpdate 간격이며 Present/CPU busy/GPU/표시 cadence가 아니다. 해당 지표·GC/할당·후보 순수 시간·시각 품질은 미측정이다.20ms p95는 미달성 목표다.

## 4. 원인 분석

확인: 기존 불변-lift 후보는 포즈 검사 자체가 성공해도 checkedLift 차이가0.1mm를 넘으면 취소했다. 실제 이전17회에서+3.8147mm가 반복됐고, 새 후보는 원래 Accept처럼 검증된 lift와 contacts를 함께 수용한다. 새 실제 대비와 적용 journal은 현재 위치를 옮기거나 완료 횟수를 조작해 통과한 것이 아님을 연결한다. 과거ID4의 완전히 같은 포즈를 재생한 것은 아니다.

가설/한계: 이 변경은 이미 유효한 등반 종료에서 불필요한 취소를 줄일 수 있다. 현재 actual 대비는 단일 기능 근거이며 일반 맵·모든 자세·8파트 화면 동기화·반복 성능을 증명하지 않는다. 자연8/64에서 후보 사용0이라면 기존 경로의 호환성 회귀만 입증한다. 다른 초기 조건의 과거 회귀 숫자 차이로 개선률을 만들지 않는다. 이력에 남은 기술적 실패와 정상/미분류 낙하를 최종 상태 성공으로 지우지 않는다.

## 5. 검증 수준

가드: 제한된 진단 통과. 실제 접촉: 위 단일 대비 결과 범위.8/64: 표의 원래 기능 gate 결과대로 판정. 반복 검증 완료·일반 최종 수용·시각 품질·성능 개선: 미완료. 서버 소유 프로세스 정리와 client exit-1을 정상 게임 종료 인증으로 간주하지 않으며 normalQuitVerified=false를 유지한다. 전체 골 active.

## 6. 변경 사항

새 격리 nf_contact_end PBO SHA256=99CE6BD581BB8204F6B5A0A67251579E928FBD446AA9B28431277880EA5A20D8. 빌드·자동 서명·DSCheckSignatures·packed script bytes 검증 성공. 각 실행 전 기존7개+후보1개 hash/서명 검증 성공. 최초 source-change-proof의 변수 재사용으로 guard/commit 발췌 라벨이 혼재한 자료는 보존하고 source-change-proof-reviewed에 올바른 계약·소스 해시를 별도로 대조했다. 해당 메타데이터 정정으로 PBO source를 바꾸지 않았다.

기존PBO·이전 후보·원본 자산·graph/native 시계·운영 서버·Steam은 변경하지 않았다. private key는 공개/배포물에 없다. 후보 source·가드·실행/분석·보고서만 기존 research 구조에 추가했다.

## 7. 원시 증거

sourceRunId: network-gameplay/contact-end-bounded-lift. candidate-lift1의 build/sign/packed/source proof, lift-guards1의50행, lift-axis-on1/off1의동일 mission·23열 result·원래state contract·14열candidate journal·9열lift commit, eight/sixtyfour-contact-end-lift1의원래 milestone/terminal/gameplay·counter·server-frame·종료와 분석을 해시 목록에 연결한다. 공개는 선정 수치/함수/대표 journal이고 원시 전체·게임자산·영상·로컬경로·프로세스·키는 제외한다.

## 8. 잔여 문제

실제 대비와 자연 회귀를 구분해 후보 활성화 빈도·남은 실패를 감사한다. 기능 통과만으로 반복/최종 수용을 선언하지 않는다. 물리 질의 비용과 애니메이션 시각 품질은 별도로 계속해야 하며 이 기능 변경을 비용 절감으로 보고하지 않는다. 기존 scratch/SupportFast의 통제 근거와 network 표시 경로를 보존하고, 다음 비용 실험은 새 측정이 구현 선택을 바꾸는 조건부터 진행한다. 추가 native buffer/캐시/보간/LOD를 무작정 섞지 않는다.

로컬 checkpoint SHA256: f5ec5a6c84bd95aac007bfa3e1e17cd8f0aad696caaae1f97b68339f855b77f1
