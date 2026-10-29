# 최신 작업: 낙하 episode 재분류: 실제 4/4 기존 복구, 후보 배포 보류

실제4개거부episode는기존코드로4/4복구.복구OFF고정포즈후보3/4LAND2회반복은실제개선근거아님.후보미배포·전체미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 실제 낙하 입력 재현과 접평면 후보: 2/4 부분 통과

실제4건8검사 재현. 고정입력 후보2회 중2/4안전이동·2/4새접촉거부. 배포변경없음·전체회귀/시각/성능미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 실제 접촉 이력 확보: 8·64개체 기록과 착지 거부 원인

실제접촉200/200연결·D61D서명/엔진검증.최종8/8·64/64이나고지대25/27미도달,전경64p95149ms.4개관절0거부근거확보.기능전체/시각/성능미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 회전과 이동으로도 복구하지 못한 이유: 검색 한도와 초기 접촉 상태

회전+1cm 복구 검색3조건 모두512state 한도.초기4접촉 통제 후 결정 동일.실제 복구 적용0·미해결.다음은실제 previousContacts/dt 읽기 전용 계측.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 연속 복구 진단: 작은 걸음과 경로 재생도 착지하지 못한 이유

연속 복구 진단:1cm 후보와7걸음 경로도 마지막 정체/실제 착지 미해결.전신 하강26개 관절0 거부,512node 한도.런타임 변경 없음.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 착지 전 정체17번: 옆면 충돌과 안전한1cm 방향 진단

착지 전17번 FOOT_X_SUPPORT 재현.1cm 이동5/72 통과·432질의 반복 일치.연속 복구는 미검증.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 64명 비용 분리 — FOLLOW·접촉 집중,착지 전 정체17번 확인

64명 고정 자극 비용 진단:p95 OFF219/ON241ms. FOLLOW/접촉에 비용 집중. ON17번 착지 전 정체. 최적화 미적용·미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 착지 복구 실제 8→64 회귀 — 최종64 도착·고지대28 미도달·p95 181ms

착지 복구 owner 구현.8명 최종8/8·고지대미도달0.64명 최종64/64·고지대미도달28·등반종료실패0.시각/성능 미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 착지 후 포즈 전환 후보와 단일 NPC 실제 화면 재관측

동결 포즈 후보1.85초/1.09m 안전 경계 통과. 단일NPC 실제 영상과CPU 재관측.8/64·시각·성능 미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 착지 전후 포즈의 탈출 가능성 차이 — 안전 경로2개 재실행

동일root·방향·lift에서낙하포즈안전탈출2경로재실행통과.착지후포즈는제한탐색에서출구미발견.단순착지보류0/4로미채택.새live/시각/성능수용없음.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 지면 출구 검사 누락 재현 — 복구0/3으로 후보 미채택

정적 ground handoff 누락3건 재현. 후보는 잘못된 완료를 막았지만 복구0/3,미세 직선0/12로 기본OFF·미채택. 새8/64·애니메이션·성능 미측정.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 등반 종료의 법선 거부 재현 — 후보 개입 성공과63/64 정체 분리

정적3개 위치·64행 법선 원인2회 재현. 후보ON 64명 최종63/64,고지대 미도달14,등반 종료 실패0. 시각/성능 미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 실제 착지 게이트·같은 상태 재시도 — 64명 종료 검사1건 재현

8/8·64/64 최종 도착.64명 고지대17명 미도달·등반 종료1건. 실제 착지 게이트와247회 재시도 포착. 콜백0.605배, 시각/성능 미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 64명 콜백 시간0.497배: 미도달 유형과 중첩 비용 분리

64명 기존 기록:콜백 시간0.497배. 미도달24명은23명 등반 미시작+44번 늦은 완료. 중첩 비용 분리, 새 최적화/시각 수용 없음.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 확장 복구 종료 검사와 실제8/64명 시험: 고지대 미도달·성능 미수용

확장 복구의 목표·종료 검사 연결. 고정 포즈4개 안전 종료, 실제8/64 전체 경로 결과와 잔여 실패 별도 기록.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 43번 출구 복구: 범위 확장 뒤 조기 종료 재현

43번 고정 포즈: 계획0/5→4/5, 실행은 조기 종료4·stall1. 안전한 handoff0으로 후보 미채택.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 활성 창400표본 시간 회계 재현 — 수정 한계와43번 출구 실패 분리

활성400표본에서입력7969ms/graph5150ms/초과분2853ms.시간회계2회재현,수정·시각·64수용미완료.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 같은 호출의 입력·누적기·그래프 시간 회계 — 느린 진행 구간 확인

동일호출입력7964ms/graph5250ms/초과분2747ms.읽기진단1회,원속도/시각/64기능·성능미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 표시 비율 후보 거부 — 같은6초 영상의 세대경계 점프

기존 영상6초씩 재분석: 후보 큰 점프5개 모두 세대경계. previous-period 표시 후보 거부. 평균속도·64기능/성능 미해결.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 8부품 표시 시간 대조와 단일 비율 후보 — 평균 속도 미해결

8부품 표시 비율 진단: 대조/후보 각1회 정상 종료. 약0.64 graph 속도는 지속, 시각/성능 미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: DayZ 동일 입력 복사본2회·원본 대조와 수집기 경합 수정

DayZ에서 내용이 같은 입력 복사본799호출·2회 정상 실행을 확인했다. 표시 비율 후보를 위한 기초 호환성 근거이며 속도·부드러움 개선은 아니다.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 애니메이션 속도 제어 범위·복사본 진단 준비와 사용자 시험 예외

평균 재생 속도와 표시 보간을 구분했다. 지원 제어 범위와 복사본 도구48000호출을 검증했지만 DayZ 개선은 아직 미실행이며 사용자 시험의 별도 UI 예외를 보존했다.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 애니메이션 갱신 경계와 실제 화면 연결 — 엔진 시계 원점2회 검증

두 번의 정밀 연결에서590개 native/CPU/영상쌍을 확보했다. reset 경계34ms 입력에 몸 진행은 약3~4ms였다. 원인 기여 근거이며 수정 성공은 아니다.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 네이티브 갱신 시각·시계 원점 교정과 분석기 경계 수정

실제1,199호출과 정확 float32 경계를 검증했다. 시계 원점 차이는 측정했지만7.5ms 불확실성 때문에 native/화면 직접 연결은 아직 미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 달리기 포즈 버퍼 연속성 2회와 CPU·실제 영상 연결

1개체 달리기2회,480개 보간 쌍 교체 연속. CPU/실제 화면 연결을 반복 확인했고 끊김 원인은 아직 미확정.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 정상 표시8→64 및 실제 전체 복구 검사 수동 질의

원래 검사 수동 질의 총12묶음, 복원 위반0, 정체 표본에서 추가만 통과0. 이동·시각·성능 해결은 미완료.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 창 활성화와 미션 갱신 저하 분리: 수동 복귀 질의는 미실행

수동 질의는0회. 원래 미션의 창 활성화 전259ms·후20ms 중앙값을 관측. IsAppActive 단독 판단의 반례 확보; 이동·애니메이션·성능 해결은 미완료.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: 착지 차단과 첫 걸음 분리: B8FB ON/OFF 및 정적 재생

같은 B8FB ON/OFF 단회 고지대 미도달 2/5. 정적 6/6 발 지지 거부 재현, 32번은 8방향 밖 몸통 통과 각도 발견. 게임 수정·시각·성능 수용은 미완료.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# 최신 작업: B8FB 실제 재접근 검증

8/8·64/64 최종 도착, 고지대 6·32번 미도달. 64명 미션 p95 72ms, 시각/성능 미수용.

[최신 보고서](reports/operations-latest.md) · [AI 검토](AI_REVIEW.md) · [JSON](api/public/status.json)

이하 이전 공개 운영 안내는 이력이다.

# DayZ NPC — 작업 기록·공개 연구 자료

앞으로 이 GitHub 저장소(B안)만 사용합니다. GPT 대시보드는 운영을 종료합니다.

## 먼저 읽을 문서

- [최신 작업 완료 기록](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations-latest.md)
- [최신 실험 요약](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/latest.md)
- [AI에게 전달할 단일 검토 문서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/AI_REVIEW.md)
- [게시·접근 검증 상태](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/verification/latest.json)

## 공개 데이터와 근거

- [상태 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/status.json)
- [실험 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/experiments.json)
- [지표 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/metrics.json)
- [기능 함수·대표 로그](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/downloads/admission-review.txt)
- [애니메이션 함수·대표 로그](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/downloads/animation-review.txt)
- [파일 SHA-256 목록](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/manifest.json)

## 갱신 원칙

의미 있는 작업/실험 묶음이 안전하게 끝나면 로컬 체크포인트를 작성하고, 공개 가능한 JSON·보고서·선정된 함수/로그 발췌를 이 저장소에 갱신합니다. 완료 답변에는 해당 B안 링크와 고정 커밋 링크를 함께 제공합니다. 새 도구/피드백/누락·보완점/다음 실험 계획도 보고서에 포함합니다.

작업 완료 기록과 게임 실험 결과는 구분합니다. generatedAt/exportedAt은 새 게임 실험 수행 시각이 아닙니다. 현재 UTC가 자료의 staleAfter를 넘으면 오래된 자료입니다. 진단 성공·실험 성공·최종 수용을 따로 판정하고, 일반 HTTP200을 모든 ChatGPT 세션의 읽기 성공으로 간주하지 않습니다.

## 공개 범위

전체 원시 로그·영상이 아닌 선정된 공개 자료입니다. 원본 설치 자산, 비공개 원시 기록, 게임 코드 전체, 자격증명, 로컬 경로·프로세스 정보는 포함하지 않습니다. 영상 품질이나 GPU/Present 시각을 텍스트만으로 검증했다고 주장하지 않습니다.

JSON의 `/downloads/...` 같은 canonical URI는 `https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main` 뒤에 붙여 해석합니다. 과거 보고서의 이전 대시보드 언급은 당시 이력이며 현재 진입점이 아닙니다. 현재 ChatGPT 웹 도구의 새 Raw URL Cache miss는 검증 기록에 미해결로 남깁니다.
