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
